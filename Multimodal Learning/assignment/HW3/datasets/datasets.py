def create_dataset(cfg, split='train'):
    dataset = None
    data_loader = None
    if cfg.data.dataset == 'rcc_dataset':
        from datasets.rcc_dataset import RCCDataset, RCCDataLoader

        dataset = RCCDataset(cfg, split)
        data_loader = RCCDataLoader(
            dataset,
            batch_size=dataset.batch_size,
            shuffle=True if split == 'train' else False,
            num_workers=cfg.data.num_workers,
            pin_memory=True)
    elif cfg.data.dataset == 'rcc_dataset_spot':
        from datasets.rcc_dataset_spot import RCCDataset, RCCDataLoader
        dataset = RCCDataset(cfg, split)
        data_loader = RCCDataLoader(
            dataset,
            batch_size=dataset.batch_size,
            shuffle=True if split == 'train' else False,
            num_workers=cfg.data.num_workers,
            pin_memory=True)

    elif cfg.data.dataset == 'rcc_dataset_transformer':
        from datasets.rcc_dataset_transformer import RCCDataset, RCCDataLoader
        dataset = RCCDataset(cfg, split)
        data_loader = RCCDataLoader(
            dataset,
            batch_size=dataset.batch_size,
            shuffle=True if split == 'train' else False,
            num_workers=cfg.data.num_workers,
            pin_memory=True)
    elif cfg.data.dataset == 'rcc_dataset_transformer_with_entity':
        from datasets.rcc_dataset_transformer_with_entity import RCCDataset, RCCDataLoader
        dataset = RCCDataset(cfg, split)
        data_loader = RCCDataLoader(
            dataset,
            batch_size=dataset.batch_size,
            shuffle=True if split == 'train' else False,
            num_workers=cfg.data.num_workers,
            pin_memory=True)
    elif cfg.data.dataset == 'rcc_dataset_transformer_with_dep':
        from datasets.rcc_dataset_transformer_with_dep import RCCDataset, RCCDataLoader
        dataset = RCCDataset(cfg, split)
        data_loader = RCCDataLoader(
            dataset,
            batch_size=dataset.batch_size,
            shuffle=True if split == 'train' else False,
            num_workers=cfg.data.num_workers,
            pin_memory=True)
    elif cfg.data.dataset == 'rcc_dataset_transformer_dc':
        from datasets.rcc_dataset_transformer_dc import RCCDataset, RCCDataLoader
        dataset = RCCDataset(cfg, split)
        data_loader = RCCDataLoader(
            dataset,
            batch_size=dataset.batch_size,
            shuffle=True if split == 'train' else False,
            num_workers=cfg.data.num_workers,
            pin_memory=True)
    elif cfg.data.dataset == 'rcc_dataset_transformer_spot':
        from datasets.rcc_dataset_transformer_spot import RCCDataset, RCCDataLoader
        dataset = RCCDataset(cfg, split)
        data_loader = RCCDataLoader(
            dataset,
            batch_size=dataset.batch_size,
            shuffle=True if split == 'train' else False,
            num_workers=cfg.data.num_workers,
            pin_memory=True)
    elif cfg.data.dataset == 'rcc_dataset_spot_transformer':
        from datasets.rcc_dataset_transformer_spot import RCCDataset, RCCDataLoader
        dataset = RCCDataset(cfg, split)
        data_loader = RCCDataLoader(
            dataset,
            batch_size=dataset.batch_size,
            shuffle=True if split == 'train' else False,
            num_workers=cfg.data.num_workers,
            pin_memory=True)
    elif cfg.data.dataset == 'rcc_dataset_transformer_edit':
        from datasets.rcc_dataset_transformer_edit import RCCDataset, RCCDataLoader
        dataset = RCCDataset(cfg, split)
        data_loader = RCCDataLoader(
            dataset,
            batch_size=dataset.batch_size,
            shuffle=True if split == 'train' else False,
            num_workers=cfg.data.num_workers,
            pin_memory=True)
    elif cfg.data.dataset == 'rcc_dataset_transformer_with_dep_edit':
        from datasets.rcc_dataset_transformer_with_dep_edit import RCCDataset, RCCDataLoader
        dataset = RCCDataset(cfg, split)
        data_loader = RCCDataLoader(
            dataset,
            batch_size=dataset.batch_size,
            shuffle=True if split == 'train' else False,
            num_workers=cfg.data.num_workers,
            pin_memory=True)
    else:
        raise Exception('Unknown dataset: %s' % cfg.data.dataset)
    
    return dataset, data_loader
