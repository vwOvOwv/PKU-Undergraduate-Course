import math
import sys

def train_one_epoch(model, optimizer, data_loader, device, count, writer):
    model.train()

    lr_scheduler = None
    for images, targets in data_loader:
        images = list(image.to(device) for image in images)
        targets = [{k: v.to(device) for k, v in t.items()} for t in targets]

        loss_dict = model(images, targets)

        losses = sum(loss for loss in loss_dict.values())
        loss_value = losses.item()
        if not math.isfinite(loss_value):
            print("Loss is {}, stopping training".format(loss_value))
            sys.exit(1)

        optimizer.zero_grad()
        losses.backward()
        optimizer.step()

        if lr_scheduler is not None:
            lr_scheduler.step()

        print("Iteration: {}, Loss: {}".format(count, loss_value))
        writer.add_train_scalar("Loss", loss_value, count)
        for key, value in loss_dict.items():
            writer.add_train_scalar(key, value.item(), count)
        
        count += 1
    return count
