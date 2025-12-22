import torch
import torch.nn as nn
import torch.nn.functional as F


def recall(actual, predicted, k):
    act_set = set([actual])
    pred_set = set(predicted[:k])
    result = len(act_set & pred_set) / float(len(act_set))
    return result


def compute_score(solution, prediction):
    n = len(solution)
    scores_r_10, scores_r_50 = [], []
    for i in range(n):
        assert solution[i]["candidate"] == prediction[i]["candidate"]

        scores_r_10.append(recall(solution[i]["target"], prediction[i]["ranking"], 10))
        scores_r_50.append(recall(solution[i]["target"], prediction[i]["ranking"], 50))

    return sum(scores_r_10) / n, sum(scores_r_50) / n


def sharded_cross_view_inner_product(trg_embds, src_embds, subspaces=None,
                                     l2renorm=False, tol=1e-5, dist=False, val=False):
    '''
    args
      trg_embds: {attr0: B x N x D or N x D}
      src_embds: {attr0: B x D}
    '''
    B = src_embds.size(0)

    device = trg_embds.device
    trg_dim_idx = len(trg_embds.size()) - 1  # 2 or 1
    N = trg_embds.size(trg_dim_idx - 1)
    # B x N
    sims = torch.zeros((B, N), device=device)

    if l2renorm:
        l2_mass_trg, l2_mass_src = 0, 0
        for idx, modality in enumerate(subspaces):
            trg_embd_ = trg_embds[modality]  # B x N x D or N x D
            l2_mass_trg += trg_embd_.pow(2).sum(trg_dim_idx)
            src_embd_ = src_embds[modality]  # B x D
            l2_mass_src += src_embd_.pow(2).sum(1)
        l2_mass_trg = torch.sqrt(l2_mass_trg.clamp(min=1e-6)).unsqueeze(trg_dim_idx)
        l2_mass_src = torch.sqrt(l2_mass_src.clamp(min=1e-6)).unsqueeze(1)
    else:
        l2_mass_trg, l2_mass_src = 1, 1

    # for idx, modality in enumerate(1):
    trg_embd_ = trg_embds / l2_mass_trg  # B x N x D or N x D
    src_embd_ = src_embds / l2_mass_src  # B x D
    if dist:
        sims += (trg_embd_ - src_embd_.unsqueeze(1)).pow(2).sum(2)  # B x N
    else:
        if trg_dim_idx == 2:
            tmp = torch.matmul(trg_embd_, src_embd_.unsqueeze(-1))  # B x N x 1
            sims += tmp.squeeze(-1)  # B x N
        else:
            sims += torch.matmul(src_embd_, trg_embd_.t())  # B x N

    if torch.isnan(sims).sum().item():
        import ipdb;
        ipdb.set_trace()
        raise ValueError("Found nans in similarity matrix!")

    return sims


def extended_inner_product(trg_embds, src_embds, subspaces):
    '''
    args  only for test
      trg_embds: {attr0: B x B x D or B x D}
      src_embds: {attr0: B x D}
    '''
    B = src_embds[subspaces[0]].size(0)

    device = trg_embds[subspaces[0]].device
    trg_dim_idx = len(trg_embds[subspaces[0]].size()) - 1  # 2 or 1
    # B x 2B
    sims = torch.zeros((B, 2 * B), device=device)

    for idx, modality in enumerate(subspaces):
        src_embd_ = src_embds[modality]  # B x D
        trg_embd_ = trg_embds[modality]  # B x B x D or B x D
        if trg_dim_idx == 2:
            tmp = torch.matmul(trg_embd_, src_embd_.unsqueeze(-1))  # B x N x 1
            tmp = tmp.squeeze(-1)  # B x N
            tmp2 = torch.matmul(src_embd_, src_embd_.t())  # B x B
            sims += torch.cat((tmp, tmp2), 1)  # B x 2B
        else:
            trg_embd_ = torch.cat((trg_embd_, src_embd_), 0)  # 2B x D
            sims += torch.matmul(src_embd_, trg_embd_.t())  # B x 2B

    msk = torch.diag(torch.ones(B, device=device) * float('-inf'), diagonal=B)  # 2B x 2B
    msk = msk[:B]  # B x 2B
    sims += msk

    if torch.isnan(sims).sum().item():
        import ipdb;
        ipdb.set_trace()
        raise ValueError("Found nans in similarity matrix!")

    return sims


class InnerProduct(nn.Module):
    def __init__(self):
        super().__init__()

    def forward(self, trg_embds, src_embds, subspaces=None,
                l2renorm=False, tol=1e-5, dist=False, val=False):
        sims = sharded_cross_view_inner_product(
            trg_embds, src_embds, subspaces,
            l2renorm, tol, dist, val)
        return sims


class InnerProductStill(nn.Module):
    def __init__(self):
        super().__init__()

    def forward(self, trg_embds, src_embds, subspaces, **args):
        sims = sharded_cross_view_inner_product(
            trg_embds, src_embds, subspaces)
        return sims


class ExtendedProduct(nn.Module):
    def __init__(self):
        super().__init__()

    def forward(self, trg_embds, src_embds, subspaces,
                l2renorm=False, tol=1e-5, dist=False, val=False):
        if val:
            sims = sharded_cross_view_inner_product(
                trg_embds, src_embds, subspaces,
                l2renorm, tol, dist, val)
        else:
            sims = extended_inner_product(
                trg_embds, src_embds, subspaces
            )  # 2B x 2B
        return sims