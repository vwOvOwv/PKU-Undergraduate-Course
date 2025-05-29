from rnn_layers import rnn_step_forward, rnn_step_backward
import numpy as np


def check_answer(answer_dict, gt_answer_dict):
    for key in answer_dict.keys():
        if key not in gt_answer_dict:
            print('Unrecognized key %s in answer_dict' % key)
            return False
        if not np.allclose(answer_dict[key], gt_answer_dict[key], atol=1e-6, rtol=1e-5):
            print('The %s does not match the gt_answer.' % key)
            return False
    print('All items in the answer_dict match the gt_answer!')
    return True


def unit_test(x, prev_h, Wx, Wh, b):
    next_h, cache = rnn_step_forward(x, prev_h, Wx, Wh, b)
    dx, dprev_h, dWx, dWh, db = rnn_step_backward(dnext_h, cache)
    answer_dict = {
        'next_h': next_h,
        'dx': dx,
        'dprev_h': dprev_h,
        'dWx': dWx,
        'dWh': dWh,
        'db': db
    }
    return answer_dict

# ------------------------------------------------------------------- #
# You can use the following example to check your implementation of   #
# rnn_step_forward and rnn_step_backward.                             #
# ------------------------------------------------------------------- #
N, D, H = 3, 10, 4

x = np.linspace(-0.4, 0.7, num=N*D).reshape(N, D)
prev_h = np.linspace(-0.2, 0.5, num=N*H).reshape(N, H)
Wx = np.linspace(-0.1, 0.9, num=D*H).reshape(D, H)
Wh = np.linspace(-0.3, 0.7, num=H*H).reshape(H, H)
b = np.linspace(-0.2, 0.4, num=H)
dnext_h = np.linspace(-3, 3, num=N*H).reshape(N, H)

answer_dict = unit_test(x, prev_h, Wx, Wh, b)
gt_answer_dict = np.load('results/reference_single_rnn_layer.npy', allow_pickle=True).item()
check_answer(answer_dict, gt_answer_dict)

# ------------------------------------------------------------------- #
# Now, we run your code again and get your results for fair evaluation.
# -------------------------------------------------------------------#
np.random.seed(233)
x = np.random.randn(N, D)
prev_h = np.random.randn(N, H)
Wx = np.random.randn(D, H)
Wh = np.random.randn(H, H)
b = np.random.randn(H)
dnext_h = np.random.randn(N, H)

answer_dict = unit_test(x, prev_h, Wx, Wh, b)
np.save('results/single_rnn_layer.npy', answer_dict)