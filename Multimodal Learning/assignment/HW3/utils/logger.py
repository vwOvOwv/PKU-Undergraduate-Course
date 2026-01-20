import numpy as np
import os
import time
#import visdom

class Logger():
    def __init__(self, cfg, output_dir, is_train=True):
        if is_train:
            self.display_id = cfg.logger.display_id
        else:
            self.display_id = cfg.logger.display_id + 2
        self.win_size = cfg.logger.display_winsize
        self.exp_name = cfg.exp_name
        self.output_dir = output_dir
        self.is_train = is_train
        self.cfg = cfg
        #self.vis = visdom.Visdom(port=cfg.logger.display_port)
        if is_train:
            self.log_name = os.path.join(output_dir, 'train_log.txt')
            with open(self.log_name, 'a') as log_file:
                now = time.strftime("%c")
                log_file.write('========== Training Log (%s) ==========\n' % now)
        else:
            self.log_name = os.path.join(output_dir, 'eval_log.txt')
            with open(self.log_name, 'a') as log_file:
                now = time.strftime("%c")
                log_file.write('========== Evaluation Log (%s) ==========\n' % now)

    def plot_current_stats(self, epoch, counter_ratio, stats, which_plot):
        if which_plot == 'acc':
            accs = {}
            for k, v in stats.items():
                if 'acc' in k:
                    accs[k] = v
            if not hasattr(self, 'plot_accs'):
                self.plot_accs = {'X': [], 'Y': [], 'legend': list(accs.keys())}
            self.plot_accs['X'].append(epoch + counter_ratio)
            self.plot_accs['Y'].append([accs[k] for k in self.plot_accs['legend']])
            if self.is_train:
                y_label_name = 'accuracy'
            else:
                y_label_name = 'test accuracy'
            # self.vis.line(
            #     X=np.stack([np.array(self.plot_accs['X'])] * \
            #         len(self.plot_accs['legend']), 1),
            #     Y=np.array(self.plot_accs['Y']),
            #     opts={
            #         'title': self.exp_name,
            #         'legend': self.plot_accs['legend'],
            #         'xlabel': 'epoch',
            #         'ylabel': y_label_name},
            #     win=self.display_id)
        elif which_plot == 'loss':
            losses = {}
            for k, v in stats.items():
                if 'loss' in k:
                    losses[k] = v
            if not hasattr(self, 'plot_losses'):
                self.plot_losses = {'X': [], 'Y': [], 'legend': list(losses.keys())}
            self.plot_losses['X'].append(epoch + counter_ratio)
            self.plot_losses['Y'].append([losses[k] for k in self.plot_losses['legend']])
            if self.is_train:
                y_label_name = 'loss'
            else:
                y_label_name = 'test loss'
            # self.vis.line(
            #     X=np.stack([np.array(self.plot_losses['X'])] * \
            #         len(self.plot_losses['legend']), 1),
            #     Y=np.array(self.plot_losses['Y']),
            #     opts={
            #         'title': self.exp_name,
            #         'legend': self.plot_losses['legend'],
            #         'xlabel': 'epoch',
            #         'ylabel': y_label_name},
            #     win=self.display_id + 1)

    def print_current_stats(self, epoch, i, total_i, stats, t):
        message = '[Epoch: %d Iters: %d, Total Iters:%d, Time: %.3f] ' % (epoch, i, total_i, t)
        for k, v in stats.items():
            message += '%s: %.4f ' % (k, v)
        print(message)
        with open(self.log_name, 'a') as log_file:
            log_file.write('%s\n' % message)

