import numpy as np
from utils.coco_utils import load_coco_data, decode_captions
from rnn import CaptioningRNN
from utils.captioning_solver import CaptioningSolver
import matplotlib.pyplot as plt

np.random.seed(233)


# ------------------------------------------------------------------- #
# Overfit your RNN on 50 training data                                 #
# ------------------------------------------------------------------- #

small_data = load_coco_data(max_train=50)

small_rnn_model = CaptioningRNN(
    word_to_idx=small_data['word_to_idx'],
    input_dim=small_data['train_features'].shape[1],
    hidden_dim=512,
    wordvec_dim=256,
)

small_rnn_solver = CaptioningSolver(
    small_rnn_model, small_data,
    update_rule='adam',
    num_epochs=50,
    batch_size=25,
    optim_config={
     'learning_rate': 5e-3,
    },
    lr_decay=0.95,
    verbose=True, print_every=10,
)

small_rnn_solver.train()

# Plot the training losses.
plt.plot(small_rnn_solver.loss_history)
plt.xlabel('Iteration')
plt.ylabel('Loss')
plt.title('Training loss history')
plt.savefig('results/rnn_loss_history.png')
plt.close()


# ------------------------------------------------------------------- #
# Inference: please uncomment these lines after completing the sample #
# function in rnn.py.
# ------------------------------------------------------------------- #
for split in ['train', 'val']:
    for i in range(2):
        data_dict = np.load(f'datasets/samples/{split}_{i}.npy', allow_pickle=True).item()
        feature = data_dict['feature'].reshape(1, -1)
        image = plt.imread(f'datasets/samples/{split}_{i}.png')

        sample_captions = small_rnn_model.sample(feature)
        sample_captions = decode_captions(sample_captions, small_data['idx_to_word'])

        # set image size
        plt.figure(figsize=(8, 4))
        plt.imshow(image)          
        plt.title('Your prediction: %s\nGT: %s' % (sample_captions[0], data_dict['gt_caption']))
        plt.axis('off')
        plt.savefig(f'results/pred_{split}_{i}.png')
        plt.close()