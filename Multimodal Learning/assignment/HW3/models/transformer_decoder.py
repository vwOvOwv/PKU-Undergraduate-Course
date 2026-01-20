from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import torch
import torch.nn as nn
import torch.nn.functional as F
import math


class PositionEncoding(nn.Module):

    def __init__(self, n_filters=128, max_len=500):
        """
        :param n_filters: same with input hidden size
        :param max_len: maximum sequence length
        """
        super(PositionEncoding, self).__init__()
        """
        == To Implement ==
        calculates the sinusoidal positional encoding for a transformer model.
        note that on each filter dimension, there is a different sinusoid (different frequency).
        expected pe to be of shape [max_len, n_filters]. 
        you can refer to the lecture slides for the formula, or https://kazemnejad.com/blog/transformer_architecture_positional_encoding/ for a quick explanation.
        """
        # pe = ...
        pe = torch.zeros(max_len, n_filters)
        position = torch.arange(0, max_len).unsqueeze(1).float()
        div_term = torch.exp(torch.arange(0, n_filters, 2).float() * -(math.log(10000.0) / n_filters))
        pe[:, 0::2] = torch.sin(position * div_term)
        pe[:, 1::2] = torch.cos(position * div_term)
        self.register_buffer("pe", pe)  # buffer is a tensor, not a variable, (L, D)
        # raise NotImplementedError

    def forward(self, x):
        """
        :Input: (*, L, D)
        :Output: (*, L, D) the same size as input
        """
        pe = self.pe.data[:x.size(-2), :]  # (#x.size(-2), n_filters)
        extra_dim = len(x.size()) - 2
        for _ in range(extra_dim):
            pe = pe.unsqueeze(0)
        x = x + pe
        return x


def gelu(x):
    """Implementation of the gelu activation function.
        For information: OpenAI GPT"s gelu is slightly different (and gives slightly different results):
        0.5 * x * (1 + torch.tanh(math.sqrt(2 / math.pi) * (x + 0.044715 * torch.pow(x, 3))))
        Also see https://arxiv.org/abs/1606.08415
    """
    return x * 0.5 * (1.0 + torch.erf(x / math.sqrt(2.0)))

class LabelSmoothingLoss(nn.Module):
    """
    With label smoothing,
    KL-divergence between q_{smoothed ground truth prob.}(w)
    and p_{prob. computed by model}(w) is minimized.
    """
    def __init__(self, label_smoothing, tgt_vocab_size, ignore_index=-100):
        assert 0.0 < label_smoothing <= 1.0
        self.ignore_index = ignore_index
        super(LabelSmoothingLoss, self).__init__()

        self.log_softmax = nn.LogSoftmax(dim=-1)

        smoothing_value = label_smoothing / (tgt_vocab_size - 1)  # count for the ground-truth word
        one_hot = torch.full((tgt_vocab_size,), smoothing_value)
        # one_hot[self.ignore_index] = 0
        self.register_buffer("one_hot", one_hot.unsqueeze(0))

        self.confidence = 1.0 - label_smoothing

    def forward(self, output, target):
        """
        output (FloatTensor): batch_size x n_classes
        target (LongTensor): batch_size, with indices in [-1, tgt_vocab_size-1], `-1` is ignored
        """
        valid_indices = target != self.ignore_index  # ignore examples with target value -1
        target = target[valid_indices]
        output = self.log_softmax(output[valid_indices])

        model_prob = self.one_hot.repeat(target.size(0), 1)
        model_prob.scatter_(1, target.unsqueeze(1), self.confidence)
        return F.kl_div(output, model_prob, reduction="mean")



class SelfAttention(nn.Module):
    def __init__(self, cfg):
        super().__init__()
        self.hidden_size = cfg.model.transformer_decoder.att_dim
        self.num_attention_heads = cfg.model.transformer_decoder.att_head
        if self.hidden_size % self.num_attention_heads != 0:
            raise ValueError(
                "The hidden size (%d) is not a multiple of the number of attention "
                "heads (%d)" % (self.hidden_size, self.num_attention_heads))
        self.attention_head_size = int(self.hidden_size / self.num_attention_heads)
        self.all_head_size = self.num_attention_heads * self.attention_head_size

        self.query = nn.Linear(self.hidden_size, self.all_head_size)
        self.key = nn.Linear(self.hidden_size, self.all_head_size)
        self.value = nn.Linear(self.hidden_size, self.all_head_size)

        self.dropout = nn.Dropout(0.1)

    def transpose_for_scores(self, x):
        new_x_shape = x.size()[:-1] + (self.num_attention_heads, self.attention_head_size)  # (N, L, nh, dh)
        x = x.view(*new_x_shape)
        return x.permute(0, 2, 1, 3)  # (N, nh, L, dh)

    def forward(self, query_states, key_states, value_states, attention_mask=None):
        """
        Args:
            query_states: (N, Lq, D)
            key_states: (N, L, D)
            value_states: (N, L, D)
            attention_mask: (N, l)
        Returns:

        """
        """
        == To Implement ==
        calculate the self attention in Transformer. 
        note that the attention mask should be applied to the attention scores before softmax. 1 means not masked, 0 means masked.
        you should also apply dropout to the attention scores.
        """
        mixed_query_layer = self.query(query_states)
        mixed_key_layer = self.key(key_states)
        mixed_value_layer = self.value(value_states)

        query_layer = self.transpose_for_scores(mixed_query_layer)
        key_layer = self.transpose_for_scores(mixed_key_layer)
        value_layer = self.transpose_for_scores(mixed_value_layer)

        attention_scores = torch.matmul(query_layer, key_layer.transpose(-1, -2))
        attention_scores = attention_scores / math.sqrt(self.attention_head_size)

        if attention_mask is not None:
            if attention_mask.dim() == 3:
                attention_mask = attention_mask.unsqueeze(1)
            attention_scores = attention_scores + (1.0 - attention_mask) * (-1e9)
        
        attention_probs = nn.Softmax(dim=-1)(attention_scores)
        attention_probs = self.dropout(attention_probs)

        context_layer = torch.matmul(attention_probs, value_layer)

        context_layer = context_layer.permute(0, 2, 1, 3).contiguous()
        new_context_layer_shape = context_layer.size()[:-2] + (self.all_head_size,)
        context_layer = context_layer.view(*new_context_layer_shape)

        # raise NotImplementedError
        return context_layer


class CrossAttention(nn.Module):
    def __init__(self, cfg):
        super().__init__()
        self.hidden_size = cfg.model.transformer_decoder.att_dim
        self.num_attention_heads = cfg.model.transformer_decoder.att_head
        if self.hidden_size % self.num_attention_heads != 0:
            raise ValueError(
                "The hidden size (%d) is not a multiple of the number of attention "
                "heads (%d)" % (self.hidden_size, self.num_attention_heads))
        self.attention_head_size = int(self.hidden_size / self.num_attention_heads)
        self.all_head_size = self.num_attention_heads * self.attention_head_size

        self.query = nn.Linear(self.hidden_size, self.all_head_size)
        self.key = nn.Linear(self.hidden_size, self.all_head_size)
        self.value = nn.Linear(self.hidden_size, self.all_head_size)

        self.dropout = nn.Dropout(0.1)

    def transpose_for_scores(self, x):
        new_x_shape = x.size()[:-1] + (self.num_attention_heads, self.attention_head_size)  # (N, L, nh, dh)
        x = x.view(*new_x_shape)
        return x.permute(0, 2, 1, 3)  # (N, nh, L, dh)

    def forward(self, query_states, key_states, value_states, attention_mask=None):
        """
        Args:
            query_states: (N, Lq, D)
            key_states: (N, L, D)
            value_states: (N, L, D)
            attention_mask: (N, l)
        Returns:

        """
        """
        == To Implement ==
        calculate the cross attention in Transformer. 
        note that the attention mask should be applied to the attention scores before softmax. 1 means not masked, 0 means masked.
        you should also apply dropout to the attention scores, and return the mean attention scores over all heads.
        """
        mixed_query_layer = self.query(query_states)
        mixed_key_layer = self.key(key_states)
        mixed_value_layer = self.value(value_states)

        query_layer = self.transpose_for_scores(mixed_query_layer)
        key_layer = self.transpose_for_scores(mixed_key_layer)
        value_layer = self.transpose_for_scores(mixed_value_layer)

        attention_scores = torch.matmul(query_layer, key_layer.transpose(-1, -2))
        attention_scores = attention_scores / math.sqrt(self.attention_head_size)

        if attention_mask is not None:
            if attention_mask.dim() == 3:
                attention_mask = attention_mask.unsqueeze(1)
            attention_scores = attention_scores + (1.0 - attention_mask) * -1e9

        attention_probs = nn.Softmax(dim=-1)(attention_scores)
        attention_probs = self.dropout(attention_probs)

        context_layer = torch.matmul(attention_probs, value_layer)

        context_layer = context_layer.permute(0, 2, 1, 3).contiguous()
        new_context_layer_shape = context_layer.size()[:-2] + (self.all_head_size,)
        context_layer = context_layer.view(*new_context_layer_shape)

        # raise NotImplementedError
        return context_layer, attention_probs.mean(1)


class Output(nn.Module):
    def __init__(self, cfg):
        super().__init__()
        self.intermediate_size = cfg.model.transformer_decoder.att_dim
        self.hidden_size = cfg.model.transformer_decoder.att_dim
        self.dense = nn.Linear(self.intermediate_size, self.hidden_size)
        self.LayerNorm = nn.LayerNorm(self.hidden_size, eps=1e-6)
        self.dropout = nn.Dropout(0.1)

    def forward(self, hidden_states, input_tensor):
        hidden_states = self.dense(hidden_states)
        hidden_states = self.dropout(hidden_states)
        hidden_states = self.LayerNorm(hidden_states + input_tensor)
        return hidden_states


class Intermediate(nn.Module):
    def __init__(self, cfg):
        super().__init__()
        self.intermediate_size = cfg.model.transformer_decoder.att_dim
        self.dense = nn.Linear(self.intermediate_size, self.intermediate_size)
        self.intermediate_act_fn = gelu

    def forward(self, hidden_states):
        hidden_states = self.dense(hidden_states)
        hidden_states = self.intermediate_act_fn(hidden_states)
        return hidden_states


class DecoderLayer(nn.Module):
    def __init__(self, cfg):
        super().__init__()
        self.hidden_size = cfg.model.transformer_decoder.att_dim
        self.self_attention = SelfAttention(cfg)
        self.norm1 = nn.LayerNorm(self.hidden_size, eps=1e-6)
        self.dec_enc_attention = CrossAttention(cfg)
        self.hidden_intermediate = Intermediate(cfg)
        self.norm2 = nn.LayerNorm(self.hidden_size, eps=1e-6)
        self.output = Output(cfg)  # linear + residual + layernorm

    def forward(self, dec_hidden_states, dec_mask, enc_outputs, diagonal_mask=True):
        """
        Args:
            dec_hidden_states: (N, Lt, D)
            dec_mask: (N, Lt)
            enc_outputs: (N, Lv, D)
            enc_mask: (N, Lv)
            diagonal_mask: bool, if True mask subsequent words to preserve auto-regressive property.
        Returns:

        """
        self_attention_mask = dec_mask.unsqueeze(1)
        if diagonal_mask:  # mask subsequent words
            max_len = dec_mask.size(1)  # Lt
            self_attention_mask = self_attention_mask * \
                torch.tril(self_attention_mask.new_ones(max_len, max_len), diagonal=0)

        # 1, dec self attn + add_norm
        attention_output = self.self_attention(
            dec_hidden_states, dec_hidden_states, dec_hidden_states, self_attention_mask)  # (N, Lt, D)

        attention_output = self.norm1(attention_output + dec_hidden_states)  # (N, Lt, D)

        # 2, dec enc attn + add_norm
        # Is the attention mask correct?
        # Yes! Use the mask associated with key/value, not query. (query, key, value)
        # Additionally, there is no need to do subsequent masking, since each word has the right to see
        # all the video info.
        dec_enc_attention_output, attention_vis = self.dec_enc_attention(
            attention_output, enc_outputs, enc_outputs)  # (N, Lt, D)

        dec_enc_attention_output = self.norm2(attention_output + dec_enc_attention_output)  # (N, Lt, D)

        # 3, linear + add_norm
        dec_enc_attention_output = self.hidden_intermediate(dec_enc_attention_output)
        dec_enc_attention_output = self.output(dec_enc_attention_output, dec_enc_attention_output)  # (N, Lt, D)
        return dec_enc_attention_output, attention_vis  # (N, Lt, D)


class DynamicCore(nn.Module):
    def __init__(self, cfg):
        super().__init__()
        self.vocab_size = cfg.model.transformer_decoder.vocab_size
        self.word_embed_size = cfg.model.transformer_decoder.word_dim
        self.hidden_size = cfg.model.transformer_decoder.att_dim
        self.fc = nn.Sequential(
            nn.Linear(self.word_embed_size, self.hidden_size),
            nn.Dropout(0.1),
            nn.LayerNorm(self.hidden_size, eps=1e-6)
            # nn.ReLU(),
           )

        self.position_enc = PositionEncoding(n_filters=self.word_embed_size,
                                                    max_len=cfg.model.transformer_decoder.seq_length)
        # self.layernorm = nn.LayerNorm(self.hidden_size, eps=1e-6)

        self.embed = nn.Embedding(self.vocab_size, self.word_embed_size, padding_idx=0)
        self.num_hidden_layers = cfg.model.transformer_decoder.att_layer
        self.layer = nn.ModuleList([DecoderLayer(cfg)
                                    for _ in range(self.num_hidden_layers)])

    def forward(self, seq, dec_mask, enc_outputs,
                diagonal_mask=True, output_all_encoded_layers=False):
        """
        Args:
            dec_hidden_states: (N, Lt, D)
            dec_mask: (N, Lt)
            enc_outputs: (N, Lv, D)
            diagonal_mask: bool, if True mask subsequent words to preserve auto-regressive property
            output_all_encoded_layers:

        Returns:

        """
        # dec_hidden_states = self.dropout(self.position_enc(self.embed(seq)))
        # dec_hidden_states = self.layernorm(dec_hidden_states)
        dec_hidden_states = self.position_enc(self.embed(seq))
        dec_hidden_states = self.fc(dec_hidden_states)
        all_encoder_layers = []
        all_att_vis = []
        for layer_idx, layer_module in enumerate(self.layer):
            dec_hidden_states, attention_vis = layer_module(
                dec_hidden_states, dec_mask, enc_outputs, diagonal_mask=diagonal_mask)
            if output_all_encoded_layers:
                all_encoder_layers.append(dec_hidden_states)
                all_att_vis.append(attention_vis)
        if not output_all_encoded_layers:
            all_encoder_layers.append(dec_hidden_states)
            all_att_vis.append(attention_vis)
        return all_encoder_layers[-1], all_att_vis[-1]


class PredictionHeadTransform(nn.Module):
    def __init__(self, cfg):
        super().__init__()
        self.hidden_size = cfg.model.transformer_decoder.att_dim
        self.dense = nn.Linear(self.hidden_size, self.hidden_size)
        self.transform_act_fn = gelu
        self.LayerNorm = nn.LayerNorm(self.hidden_size, eps=1e-6)

    def forward(self, hidden_states):
        """(N, L, D)"""
        hidden_states = self.dense(hidden_states)
        hidden_states = self.transform_act_fn(hidden_states)
        hidden_states = self.LayerNorm(hidden_states)
        return hidden_states


class LMPredictionHead(nn.Module):
    def __init__(self, cfg, embedding_weights=None):
        super().__init__()
        # self.transform = PredictionHeadTransform(cfg)

        self.vocab_size = cfg.model.transformer_decoder.vocab_size

        self.hidden_size = cfg.model.transformer_decoder.att_dim
        self.share_wd_cls_weight = cfg.model.transformer_decoder.share_wd_cls_weight

        # The output weights are the same as the input embeddings, but there is
        # an output-only bias for each token.
        if self.share_wd_cls_weight:
            assert embedding_weights is not None, \
                "bert_model_embedding_weights should not be None " \
                "when setting --share_wd_cls_weight flag to be true"
            assert self.hidden_size == embedding_weights.size(1), \
                "hidden size has be the same as word embedding size when " \
                "sharing word embedding weight and classifier weight"
            self.decoder = nn.Linear(embedding_weights.size(1),
                                     embedding_weights.size(0),
                                     bias=False)
            self.decoder.weight = embedding_weights
        else:
            self.decoder = nn.Linear(self.hidden_size,self.vocab_size, bias=False)

        self.bias = nn.Parameter(torch.zeros(self.vocab_size))

    def forward(self, hidden_states):
        """(N, L, D)"""
        # hidden_states = self.transform(hidden_states)
        hidden_states = self.decoder(hidden_states) + self.bias
        return hidden_states  # (N, L, vocab_size)


class Speaker(nn.Module):
    def __init__(self, cfg):
        super().__init__()
        self.vocab_size = cfg.model.transformer_decoder.vocab_size
        self.word_embed_size = cfg.model.transformer_decoder.word_dim
        self.hidden_size = cfg.model.transformer_decoder.att_dim
        self.label_smoothing = cfg.model.transformer_decoder.label_smoothing

        self.seq_length = cfg.model.transformer_decoder.seq_length

        self.core = DynamicCore(cfg)

        self.share_wd_cls_weight = cfg.model.transformer_decoder.share_wd_cls_weight


        if self.share_wd_cls_weight:
            logit_weight = self.core.embed.weight

            self.logit = LMPredictionHead(cfg, logit_weight)
        else:
            self.logit = nn.Linear(self.hidden_size, self.vocab_size)

        # self.logit_layers = getattr(cfg.model.transformer_decoder, 'logit_layers', 1)
        # if self.logit_layers == 1:
        #     self.logit = nn.Linear(self.hidden_size, self.vocab_size)
        # else:
        #     self.logit = [[nn.Linear(self.hidden_size, self.hidden_size),
        #                    nn.ReLU(),
        #                    nn.Dropout(0.1)] \
        #                   for _ in range(cfg.model.speaker.logit_layers - 1)]
        #     self.logit = nn.Sequential(*(
        #             reduce(lambda x, y: x + y, self.logit) + \
        #             [nn.Linear(self.rnn_size, self.vocab_size)]))
        self.loss_func = LabelSmoothingLoss(self.label_smoothing,self.vocab_size, ignore_index=-1) \
            if self.label_smoothing > 0 else nn.CrossEntropyLoss(ignore_index=-1)

        self.apply(self.init_weights)

    def init_weights(self, module):
        """ Initialize the weights."""
        if isinstance(module, (nn.Linear, nn.Embedding)):
            # Slightly different from the TF version which uses truncated_normal for initialization
            # cf https://github.com/pytorch/pytorch/pull/5617
            module.weight.data.normal_(mean=0.0, std=0.02)
        elif isinstance(module, nn.LayerNorm):
            module.bias.data.zero_()
            module.weight.data.fill_(1.0)
        if isinstance(module, nn.Linear) and module.bias is not None:
            module.bias.data.zero_()

    def _forward(self,
                 encoder_output, seq, mask, labels_with_ignore=None):


        decoder_outputs, attention_weight = self.core(
            seq, mask, encoder_output, diagonal_mask=True)  # [:,1:,:]  # (N, Lt, D)
        prediction_scores = self.logit(decoder_outputs)
        caption_loss = 0.
        if labels_with_ignore is not None:
            caption_loss = self.loss_func(prediction_scores.view(-1, self.vocab_size),
                                          labels_with_ignore.view(-1))
        return caption_loss, prediction_scores, attention_weight, decoder_outputs

    def sample(
            self,
            encoder_outputs, start_idx=2, unk_idx=1, sample_max=0):
        """The first few args are the same to the input to the forward_step func
        Note:
            1, Copy the prev_ms each word generation step, as the func will modify this value,
            which will cause discrepancy between training and inference
            2, After finish the current sentence generation step, replace the words generated
            after the `[EOS]` token with `[PAD]`. The replaced input_ids should be used to generate
            next memory state tensor.
        """

        bsz = encoder_outputs.size(0)
        max_cap_len = self.seq_length
        text_input_ids = encoder_outputs.new_zeros((bsz, max_cap_len), dtype=torch.long)
        text_masks = encoder_outputs.new_zeros(bsz, max_cap_len).float()  # zeros
        next_symbols = torch.LongTensor([start_idx] * bsz)  # (N, )
        for dec_idx in range(max_cap_len):
            text_input_ids[:, dec_idx] = next_symbols
            text_masks[:, dec_idx] = 1
            decoder_outputs, attention_weight = self.core(text_input_ids, text_masks, encoder_outputs, diagonal_mask=True)
            pred_scores = F.log_softmax(self.logit(decoder_outputs), -1)
            # suppress unk token; (N, L, vocab_size)
            pred_scores[:, :, unk_idx] = -1e10
            if sample_max:
                next_words = pred_scores[:, dec_idx].max(1)[1]
                next_symbols = next_words
            else:
                prob_prev = torch.exp(pred_scores[:, dec_idx])
                next_words = torch.multinomial(prob_prev, 1)
                next_symbols = next_words.view(-1).long()

            if dec_idx == 0:
                unfinished = next_symbols != 3 # 3 is the end sign
            else:
                unfinished = unfinished * (next_symbols != 3)
            next_symbols = next_symbols * unfinished.type_as(next_symbols)

            # quit loop if all sequences have finished
            if unfinished.sum() == 0:
                break
        return text_input_ids, attention_weight



