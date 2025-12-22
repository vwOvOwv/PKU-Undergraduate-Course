
import os
import numpy as np

import PIL.Image as Image
import PIL.ImageColor as ImageColor
import PIL.ImageDraw as ImageDraw
import PIL.ImageFont as ImageFont
from PIL import ImageFile
ImageFile.LOAD_TRUNCATED_IMAGES = True

import skimage
from skimage.transform import resize
from skimage.io import imread
import matplotlib.pyplot as plt
import matplotlib
import matplotlib.cm as cm
matplotlib.use('Agg')

import matplotlib.gridspec as gridspec

def transparent_cmap(cmap, N=255):
    "Copy colormap and set alpha values"

    mycmap = cmap
    mycmap._init()
    mycmap._lut[:,-1] = np.linspace(0, 0.8, N+4)
    return mycmap


def visualize_att(d_img_path, q_img_path,
                  loc_att_bef, rest_att, loc_att_aft, mod_att,
                  gen_sent, gt_sents, pred, gt_chg, save_dir, prefix):
    img_basename = d_img_path.split('/')[-1]
    d_img = imread(d_img_path)
    q_img = imread(q_img_path)
    h, w, c = d_img.shape
    if d_img is None:
        print('img not found: %s' % d_img_path)
    if q_img is None:
        print('img not found: %s' % q_img_path)

    loc_bef_cmap = transparent_cmap(plt.cm.Blues)
    rest_cmap = transparent_cmap(plt.cm.Greens)
    loc_aft_cmap = transparent_cmap(plt.cm.Reds)

    loc_att_bef = np.squeeze(loc_att_bef).astype(np.float64)
    rest_att = np.squeeze(rest_att).astype(np.float64)
    loc_att_aft = np.squeeze(loc_att_aft).astype(np.float64)
    loc_att_bef = resize(loc_att_bef, (h, w), order=3)
    rest_att = resize(rest_att, (h, w), order=3)
    loc_att_aft = resize(loc_att_aft, (h, w), order=3)
    loc_att_bef = loc_att_bef / float(loc_att_bef.sum())
    rest_att = rest_att / float(rest_att.sum())
    loc_att_aft = loc_att_aft / float(loc_att_aft.sum())

    x, y = np.mgrid[0:w, 0:h]

    fig, ((ax1, ax2, ax3), (ax4, ax5, ax6), (ax7, ax8, ax9)) = plt.subplots(3, 3)
    fig.set_size_inches(15, 15)

    plt.axis('off')
    ax1.set_title('before')
    ax1.imshow(d_img)

    ax2.remove()

    plt.axis('off')
    ax3.set_title('after')
    ax3.imshow(q_img)

    plt.axis('off')
    ax4.set_title('loc att before')
    ax4.imshow(d_img)
    ax4.contourf(x, y, 255 * loc_att_bef.T, 15, cmap=loc_bef_cmap)

    plt.axis('off')
    ax5.set_title('rest att')
    ax5.imshow(d_img)
    ax5.contourf(x, y, 255 * rest_att.T, 15, cmap=rest_cmap)

    plt.axis('off')
    ax6.set_title('loc att after')
    ax6.imshow(q_img)
    ax6.contourf(x, y, 255 * loc_att_aft.T, 15, cmap=loc_aft_cmap)

    pred = -1
    gt_chg = -1
    message = 'Pred: %d / GT: %d\n' % (pred, gt_chg)
    message += gen_sent + '\n'
    message += '----------<GROUND TRUTHS>----------\n'
    for gt in gt_sents:
        message += gt + '\n'
    message += '===================================\n'
    fig.suptitle(message, fontsize=12)

    gen_sent_length = len(gen_sent.split(' '))
    mod_att = np.transpose(mod_att)[:, :gen_sent_length] # (4, seq_len)
    gs = gridspec.GridSpec(nrows=3, ncols=3)
    ax7.remove()
    ax8.remove()
    ax9.remove()
    axbig = fig.add_subplot(gs[2, :])

    axbig.imshow(mod_att, interpolation='nearest', cmap='YlGn')
    axbig.set_xticks(range(196))
    # axbig.set_yticklabels(['bef', 'diff', 'aft'],fontsize=27)
    axbig.set_yticks(range(gen_sent_length))
    axbig.set_yticklabels(gen_sent.split(' '), rotation=45,fontsize=10)

    axbig.grid()
    axbig.set_ylabel('Module Weights')
    axbig.set_xlabel('Generated Sentence')

    plt.show()
    fig.savefig(os.path.join(save_dir, prefix + img_basename), bbox_inches='tight')
    plt.close(fig)


def draw_bounding_box_on_image(image,
                               ymin,
                               xmin,
                               ymax,
                               xmax,
                               color='red',
                               thickness=2,
                               display_str_list=(),
                               use_normalized_coordinates=True):
    """Adds a bounding box to an image.
    Bounding box coordinates can be specified in either absolute (pixel) or
    normalized coordinates by setting the use_normalized_coordinates argument.
    Each string in display_str_list is displayed on a separate line above the
    bounding box in black text on a rectangle filled with the input 'color'.
    If the top of the bounding box extends to the edge of the image, the strings
    are displayed below the bounding box.
    Args:
      image: a PIL.Image object.
      ymin: ymin of bounding box.
      xmin: xmin of bounding box.
      ymax: ymax of bounding box.
      xmax: xmax of bounding box.
      color: color to draw bounding box. Default is red.
      thickness: line thickness. Default value is 4.
      display_str_list: list of strings to display in box
                        (each to be shown on its own line).
      use_normalized_coordinates: If True (default), treat coordinates
        ymin, xmin, ymax, xmax as relative to the image.  Otherwise treat
        coordinates as absolute.
    """
    draw = ImageDraw.Draw(image)
    im_width, im_height = image.size
    if use_normalized_coordinates:
        (left, right, top, bottom) = (xmin * im_width, xmax * im_width,
                                      ymin * im_height, ymax * im_height)
    else:
        (left, right, top, bottom) = (xmin, xmax, ymin, ymax)
    draw.line([(left, top), (left, bottom), (right, bottom),
               (right, top), (left, top)], width=thickness, fill=color)
    try:
        font = ImageFont.truetype('arial.ttf', 24)
    except IOError:
        font = ImageFont.load_default()

    # If the total height of the display strings added to the top of the bounding
    # box exceeds the top of the image, stack the strings below the bounding box
    # instead of above.
    display_str_heights = [font.getsize(ds)[1] for ds in display_str_list]
    # Each display_str has a top and bottom margin of 0.05x.
    total_display_str_height = (1 + 2 * 0.05) * sum(display_str_heights)

    if top > total_display_str_height:
        text_bottom = top
    else:
        text_bottom = bottom + total_display_str_height
    # Reverse list and print from bottom to top.
    for display_str in display_str_list[::-1]:
        text_width, text_height = font.getsize(display_str)
        margin = np.ceil(0.05 * text_height)
        draw.rectangle(
            [(left, text_bottom - text_height - 2 * margin), (left + text_width,
                                                              text_bottom)],
            fill=color)
        draw.text(
            (left + margin, text_bottom - text_height - margin),
            display_str,
            fill='black',
            font=font)
        text_bottom -= text_height - 2 * margin


def draw_bounding_box_on_image_array(image,
                                     ymin,
                                     xmin,
                                     ymax,
                                     xmax,
                                     color='red',
                                     thickness=2,
                                     display_str_list=(),
                                     use_normalized_coordinates=True):
    """Adds a bounding box to an image (numpy array).
    Bounding box coordinates can be specified in either absolute (pixel) or
    normalized coordinates by setting the use_normalized_coordinates argument.
    Args:
      image: a numpy array with shape [height, width, 3].
      ymin: ymin of bounding box.
      xmin: xmin of bounding box.
      ymax: ymax of bounding box.
      xmax: xmax of bounding box.
      color: color to draw bounding box. Default is red.
      thickness: line thickness. Default value is 4.
      display_str_list: list of strings to display in box
                        (each to be shown on its own line).
      use_normalized_coordinates: If True (default), treat coordinates
        ymin, xmin, ymax, xmax as relative to the image.  Otherwise treat
        coordinates as absolute.
    """
    image_pil = Image.fromarray(np.uint8(image)).convert('RGB')
    draw_bounding_box_on_image(image_pil, ymin, xmin, ymax, xmax, color,
                               thickness, display_str_list,
                               use_normalized_coordinates)
    np.copyto(image, np.array(image_pil))

def visualize_attention(image_path, sc_image_path, alphas_old, words, smooth=True):
    """
    Visualizes caption with weights at every word.
    Adapted from paper authors' repo: https://github.com/kelvinxu/arctic-captions/blob/master/alpha_visualization.ipynb
    :param image_path: path to image that has been captioned
    :param seq: caption
    :param alphas: weights
    :param rev_word_map: reverse word mapping, i.e. ix2word
    :param smooth: smooth weights?
    """
    image = Image.open(image_path)
    image = image.resize([14 * 24, 14 * 24], Image.LANCZOS)
    alphas_old = alphas_old.reshape(-1, 14, 14)
    # words = [rev_word_map[ind] for ind in seq]
    words = words.split()
    words.insert(0, 'start')
    words.append('end')
    alphas = alphas_old[:len(words)]

    for t in range(len(words)):
        if t > 50:
            break
        plt.subplot(int(np.ceil(len(words) / 5.)), 5, t + 1)

        plt.text(0, 1, '%s' % (words[t]), color='black', backgroundcolor='white', fontsize=12)
        plt.imshow(image)
        current_alpha = alphas[t, :]
        if smooth:
            alpha = skimage.transform.pyramid_expand(current_alpha, upscale=24, sigma=8)
        else:
            alpha = skimage.transform.resize(current_alpha, [14 * 24, 14 * 24])
        if t == 0:
            plt.imshow(alpha, alpha=0)
        else:
            plt.imshow(alpha, alpha=0.8)
        plt.set_cmap(cm.Greys_r)
        plt.axis('off')

    plt.show()