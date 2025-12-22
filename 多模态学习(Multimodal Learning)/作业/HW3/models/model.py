import json
import random
import numpy as np

import torch
import torch.nn as nn
import torch.nn.functional as F
import math


class Attention(nn.Module):
    def __init__(self, cfg):
        super().__init__()
        self.hidden_size = cfg.model.transformer_encoder.att_dim
        self.num_attention_heads = cfg.model.transformer_encoder.att_head
        if self.hidden_size % self.num_attention_heads != 0:
            raise ValueError(
                "The hidden size (%d) is not a multiple of the number of attention "
                "heads (%d)" % (self.hidden_size, self.num_attention_heads))
        self.attention_head_size = int(self.hidden_size / self.num_attention_heads)
        self.all_head_size = self.num_attention_heads * self.attention_head_size

        self.query = nn.Linear(self.hidden_size, self.all_head_size)
        self.key = nn.Linear(self.hidden_size, self.all_head_size)
        self.value = nn.Linear(self.hidden_size, self.all_head_size)
        self.layer_norm = nn.LayerNorm(self.hidden_size, eps=1e-6)
        self.dropout = nn.Dropout(0.1)
        self.logit_scale = nn.Parameter(torch.ones([]))

    def transpose_for_scores(self, x):
        new_x_shape = x.size()[:-1] + (self.num_attention_heads, self.attention_head_size)  # (N, L, nh, dh)
        x = x.view(*new_x_shape)
        return x.permute(0, 2, 1, 3)  # (N, nh, L, dh)

    def forward(self, query_states, key_states, value_states):
        """
        Args:
            query_states: (N, Lq, D)
            key_states: (N, L, D)
            value_states: (N, L, D)
            attention_mask: (N, Lq, L)

        Returns:

        """
        batch, L, D = query_states.size()
        """
        == To Implement ==
        calculate the attention in Transformer. 
        note that the attention mask should be applied to the attention scores before softmax. 1 means not masked, 0 means masked.
        you should also apply dropout to the attention scores, and return the mean attention scores over all heads.
        """
        #--- You should implement the linear projection here. ---#
        raise NotImplementedError
    
        #####################################
        #--- You don't need to change the code in this part. ---#
        # multi-head token-wise matching
        mask = torch.Tensor(np.ones([batch, L])).cuda()
        mask_sum = mask.sum(-1)
        retrieve_logits = torch.einsum("ahld,bhmd->ablm", query_layer, key_layer)  # (B,B,Nq,Nk)
        t2v_logits, max_idx1 = retrieve_logits.max(dim=-1)  # B,B,Nq,Nk -> B,B,Nq
        v2t_logits, max_idx2 = retrieve_logits.max(dim=-2)  # B,B,Nq,Nk -> B,B,Nk
        # Cross-view contrastive alignment
        t2v_logits = torch.sum(t2v_logits, dim=2) / (mask_sum.unsqueeze(1))
        v2t_logits = torch.sum(v2t_logits, dim=2) / (mask_sum.unsqueeze(0))
        retrieve_logits = (t2v_logits + v2t_logits) / 2.0
        logit_scale = self.logit_scale.exp()
        retrieve_logits = logit_scale * retrieve_logits
        ##################################################

        #--- You should implement the attention here ---#
        raise NotImplementedError

        #--- You don't need to change the code in this part. ---#
        context_layer += query_states
        context_layer = self.layer_norm(context_layer)
        return context_layer, retrieve_logits, attention_probs.mean(1)


class SCORER(nn.Module):
    def __init__(self, cfg):
        super().__init__()
        self.hidden_size = cfg.model.transformer_encoder.att_dim
        self.attention = Attention(cfg)

    def forward(self, q, k, v):
        """
        Args:
            dec_hidden_states: (N, Lt, D)
            dec_mask: (N, Lt)
            enc_outputs: (N, Lv, D)
            enc_mask: (N, Lv)
            diagonal_mask: bool, if True mask subsequent words to preserve auto-regressive property.
        Returns:

        """

        # 1, dec self attn + add_norm
        attention_output, contra_weight, att_weight = self.attention(
            q, k, v)  # (N, Lt, D)

        return attention_output, contra_weight, att_weight  # (N, Lt, D)

class ChangeDetector(nn.Module):

    def __init__(self, cfg):
        super().__init__()
        self.input_dim = cfg.model.transformer_encoder.input_dim
        self.dim = cfg.model.transformer_encoder.dim
        self.feat_dim = cfg.model.transformer_encoder.feat_dim

        self.att_dim = cfg.model.transformer_encoder.att_dim

        self.embed_dim = cfg.model.transformer_encoder.emb_dim

        self.img = nn.Sequential(
            nn.Conv2d(self.feat_dim, self.att_dim,kernel_size=1, padding=0),
        )

        self.w_embedding = nn.Embedding(14, int(self.att_dim / 2))
        self.h_embedding = nn.Embedding(14, int(self.att_dim / 2))
        self.num_hidden_layers = cfg.model.transformer_encoder.att_layer

        self.scorer = nn.ModuleList([SCORER(cfg)
                                    for _ in range(self.num_hidden_layers)])

        self.embed_fc = nn.Sequential(
            nn.Linear(self.att_dim*2, self.embed_dim),
            nn.Dropout(0.1),
            nn.ReLU(),
        )

    def forward(self, input_1, input_2):
        batch_size, C, H, W = input_1.size()

        input_1 = self.img(input_1)  # (128,196, 512)
        input_2 = self.img(input_2)

        pos_w = torch.arange(W).cuda()
        pos_h = torch.arange(H).cuda()
        embed_w = self.w_embedding(pos_w)
        embed_h = self.h_embedding(pos_h)
        position_embedding = torch.cat([embed_w.unsqueeze(0).repeat(W, 1, 1),
                                        embed_h.unsqueeze(1).repeat(1, H, 1)],
                                       dim=-1)

        position_embedding = position_embedding.permute(2, 0, 1).unsqueeze(0).repeat(batch_size, 1, 1,
                                                                                     1)  # (batch, d_model, h, w)
        input_1 = input_1 + position_embedding
        input_2 = input_2 + position_embedding

        input_1 = input_1.view(batch_size, self.att_dim, -1).permute(0, 2, 1)
        input_2 = input_2.view(batch_size, self.att_dim, -1).permute(0, 2, 1)
        input_pre1 = input_1
        input_pre2 = input_2

        input_1_list = []
        for layer_idx, layer_module in enumerate(self.scorer):
            # input_1, input_2 = layer_module(input_1, input_2, input_2), layer_module(input_2, input_1, input_1)
            input_1_list.append(input_1)
            input_1, contra_weight1, att_weight1 = layer_module(input_1, input_2, input_2)
            input_2, contra_weight2, att_weight2 = layer_module(input_2, input_1_list[layer_idx], input_1_list[layer_idx])

        output = torch.cat([input_1, input_2], -1)
        output = self.embed_fc(output)

        return output, contra_weight1, contra_weight2, input_pre1, input_pre2

class AddSpatialInfo(nn.Module):

    def _create_coord(self, img_feat):
        batch_size, _, h, w = img_feat.size()
        coord_map = img_feat.new_zeros(2, h, w)
        for i in range(h):
            for j in range(w):
                coord_map[0][i][j] = (j * 2.0 / w) - 1
                coord_map[1][i][j] = (i * 2.0 / h) - 1
        sequence = [coord_map] * batch_size
        coord_map_in_batch = torch.stack(sequence)
        return coord_map_in_batch

    def forward(self, img_feat):
        coord_map = self._create_coord(img_feat)
        img_feat_aug = torch.cat([img_feat, coord_map], dim=1)
        return img_feat_aug
