import numpy as np
from rnn_layers import rnn_forward, rnn_backward


def unit_test(x, h0, Wx, Wh, b, dout):
  h, cache = rnn_forward(x, h0, Wx, Wh, b)
  dx, dh0, dWx, dWh, db = rnn_backward(dout, cache)
  answer_dict = {
    'h': h,
    'dx': dx,
    'dh0': dh0,
    'dWx': dWx,
    'dWh': dWh,
    'db': db
  }
  return answer_dict


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


# ------------------------------------------------------------------- #
# You can use the following example to check your implementation of   #
# rnn_step_forward and rnn_step_backward.                             #
# ------------------------------------------------------------------- #
N, T, D, H = 2, 3, 4, 5

x = np.linspace(-0.1, 0.3, num=N*T*D).reshape(N, T, D)
h0 = np.linspace(-0.3, 0.1, num=N*H).reshape(N, H)
Wx = np.linspace(-0.2, 0.4, num=D*H).reshape(D, H)
Wh = np.linspace(-0.4, 0.1, num=H*H).reshape(H, H)
b = np.linspace(-0.7, 0.1, num=H)
dout = np.linspace(-0.1, 0.2, num=N*T*H).reshape(N, T, H)

answer_dict = unit_test(x, h0, Wx, Wh, b, dout)
gt_dict = np.load('results/reference_rnn.npy', allow_pickle=True).item()
check_answer(answer_dict, gt_dict)

# ------------------------------------------------------------------- #
# Now, we run your code again and get your results for fair evaluation.
# -------------------------------------------------------------------#
np.random.seed(233)
x = np.random.randn(N, T, D)
h0 = np.random.randn(N, H)
Wx = np.random.randn(D, H)
Wh = np.random.randn(H, H)
b = np.random.randn(H)
dout = np.random.randn(N, T, H)

answer_dict = unit_test(x, h0, Wx, Wh, b, dout)
np.save('results/rnn.npy', answer_dict)