import os
import json

# 配置仓库信息
USER = "vwOvOwv"
REPO = "PKU-Undergraduate-Course"
BASE_URL = f"https://cdn.jsdelivr.net/gh/{USER}/{REPO}"

# 全局计数器，确保 ID 绝对唯一且前端友好
id_counter = 0

def get_next_id():
    global id_counter
    id_counter += 1
    return f"n{id_counter}"

def get_folder_size(path):
    """递归计算文件夹大小（KB）"""
    total = 0
    try:
        for entry in os.scandir(path):
            if entry.is_file():
                total += entry.stat().st_size
            elif entry.is_dir():
                total += get_folder_size(entry.path) * 1024  # 转回字节
    except OSError:
        pass
    return round(total / 1024, 2)  # 返回 KB

def build_tree(path):
    """递归构建文件树"""
    tree = {"name": os.path.basename(path), "type": "folder", "children": [], "path": path}
    
    if not os.path.exists(path):
        print(f"⚠️ Warning: Path not found: {path}")
        return tree
        
    try:
        items = os.listdir(path)
    except OSError as e:
        print(f"⚠️ Error reading {path}: {e}")
        return tree

    # 排序：文件夹在前，文件名忽略大小写排序
    items.sort(key=lambda x: (not os.path.isdir(os.path.join(path, x)), x.lower()))

    folder_size = 0
    for item in items:
        # 过滤规则
        if item.startswith(".") or item in ["index.html", "gen_index.py", "CNAME", "README.md", "__pycache__"]:
            continue
            
        full_path = os.path.join(path, item)
        
        if os.path.isdir(full_path):
            child_tree = build_tree(full_path)
            folder_size += child_tree.get("size", 0)
            tree["children"].append(child_tree)
        else:
            file_size = round(os.path.getsize(full_path) / 1024, 2)
            folder_size += file_size
            tree["children"].append({
                "name": item,
                "type": "file",
                "path": full_path, 
                "size": file_size
            })
    
    tree["size"] = round(folder_size, 2)
    return tree

def process_branch_root(local_path, branch_name):
    """处理根目录，生成最终的前端数据结构"""
    print(f"Processing {local_path}...")
    raw_tree = build_tree(local_path)
    
    # 检查是否为空，如果为空可能是 checkout 失败
    if not raw_tree["children"]:
        print(f"⚠️ WARNING: No files found in {local_path}!")
        return []

    # 递归处理节点：生成ID，计算URL
    def process_node(node):
        node["id"] = get_next_id()
        
        # 计算路径 (无论文件还是文件夹)
        if "path" in node:
            rel_path = os.path.relpath(node["path"], local_path)
            # 统一路径分隔符
            safe_rel_path = "/".join(rel_path.split(os.sep))
            
            # 根目录 rel_path 为 "."，跳过
            if safe_rel_path != ".":
                # 暂存相对路径供前端搜索显示用
                node["relPath"] = safe_rel_path 
                node["urlPath"] = f"{branch_name}/{safe_rel_path}"
            
            del node["path"]
            
        if node["type"] == "folder":
            for child in node["children"]:
                process_node(child)

    process_node(raw_tree)
    return raw_tree["children"]

# 执行扫描
data_cs = process_branch_root("docs/computer-science", "ComputerScience")
data_ls = process_branch_root("docs/life-science", "LifeScience")

# 计算根目录大小
def calculate_folder_size(children):
    total = 0
    for item in children:
        total += item.get("size", 0)
    return round(total, 2)

full_data = [
    {"name": "Computer Science", "type": "folder", "children": data_cs, "id": "root_cs", "size": calculate_folder_size(data_cs)},
    {"name": "Life Science", "type": "folder", "children": data_ls, "id": "root_ls", "size": calculate_folder_size(data_ls)}
]

# HTML 模板
html_template = """
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Peiyu's Course Zoo</title>
    <link rel="icon" href="data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 100 100'><text y='.9em' font-size='90'>📚</text></svg>">
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jszip/3.10.1/jszip.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/FileSaver.js/2.0.5/FileSaver.min.js"></script>
    <style>
        :root { --primary: #0366d6; --bg: #f6f8fa; --border: #e1e4e8; --hover: #f1f8ff; --selected: #e1ecf4; }
        * { box-sizing: border-box; }
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif; margin: 0; color: #24292e; display: flex; height: 100vh; overflow: hidden; background: #fff; }
        
        /* === 布局 === */
        .sidebar { width: 450px; border-right: 1px solid var(--border); background: #fcfcfc; display: flex; flex-direction: column; flex-shrink: 0; }
        .main-view { flex-grow: 1; display: flex; flex-direction: column; background: #fff; min-width: 0; }

        /* === 顶部栏 === */
        /* 1. 这是右侧顶部栏 (你之前不小心删掉的) */
        .header { height: 60px; border-bottom: 1px solid var(--border); display: flex; align-items: center; padding: 0 20px; justify-content: space-between; flex-shrink: 0; }
        .header h1 { font-size: 18px; margin: 0; white-space: nowrap; font-weight: bold; }
        .header-right { display: flex; align-items: center; gap: 15px; }
        .search-wrapper { position: relative; width: 250px; }

        /* 2. 这是左侧顶部栏 (与右侧对齐) */
        .sidebar-title { 
            height: 60px; /* 关键：与右侧 .header 高度一致 */
            display: flex; 
            align-items: center; 
            padding: 0 20px; 
            font-size: 18px; /* 字体大小与右侧 h1 保持一致 */
            font-weight: bold; 
            color: #24292e; 
            border-bottom: 1px solid var(--border); 
            background: #fcfcfc;
            flex-shrink: 0;
        }
        
        .search-wrapper { flex-grow: 1; max-width: 400px; position: relative; }
        .search-box { width: 100%; padding: 8px 12px 8px 30px; border: 1px solid var(--border); border-radius: 6px; font-size: 14px; background: #f6f8fa; transition: .2s; }
        .search-box:focus { background: #fff; border-color: var(--primary); outline: none; box-shadow: 0 0 0 3px rgba(3,102,214,0.1); }
        .search-icon { position: absolute; left: 8px; top: 50%; transform: translateY(-50%); color: #999; font-size: 14px; pointer-events: none; }

        .btn { background-color: #2ea44f; color: white; border: 1px solid rgba(27,31,35,.15); padding: 6px 16px; font-size: 14px; font-weight: 600; border-radius: 6px; cursor: pointer; display: flex; align-items: center; gap: 6px; white-space: nowrap; }
        .btn:disabled { background-color: #94d3a2; cursor: not-allowed; opacity: 0.7; }
        .btn:hover:not(:disabled) { background-color: #2c974b; }

        /* === 侧边栏树 === */
        .tree-container { flex-grow: 1; overflow-y: auto; overflow-x: auto; padding: 10px 0; }
        .tree-item { padding: 6px 15px 6px 10px; cursor: pointer; white-space: nowrap; display: flex; align-items: center; font-size: 14px; color: #444; user-select: none; }
        .tree-item:hover { background-color: var(--hover); }
        .tree-item.active { background-color: var(--selected); color: var(--primary); font-weight: 500; }
        .tree-toggle { width: 20px; text-align: center; color: #999; font-size: 10px; transition: transform 0.15s; margin-right: 4px; display: inline-flex; justify-content: center; }
        .tree-toggle.open { transform: rotate(90deg); }
        .tree-toggle.invisible { visibility: hidden; }
        .tree-icon { margin-right: 6px; color: #54aeff; }

        /* === 主视图表格 === */
        .file-list { flex-grow: 1; overflow-y: auto; }
        table { width: 100%; border-collapse: collapse; table-layout: fixed; }
        th { text-align: left; padding: 10px 15px; border-bottom: 1px solid var(--border); background: #fafbfc; color: #586069; font-size: 13px; font-weight: 600; position: sticky; top: 0; z-index: 10; }
        td { padding: 8px 15px; border-bottom: 1px solid #eaecef; font-size: 14px; vertical-align: middle; white-space: nowrap; color: #24292e; }
        td.col-name { overflow: hidden; text-overflow: ellipsis; }
        tr { cursor: default; }
        tr:hover { background-color: var(--hover); }
        tr.selected { background-color: var(--selected); }

        .col-check { width: 40px; text-align: center; overflow: visible; }
        .col-icon { width: 36px; text-align: center; font-size: 16px; }
        .col-name { width: auto; }
        .col-size { width: auto; text-align: right;}
        
        .path-hint { display: block; font-size: 11px; color: #888; margin-top: 2px; }
        .path-hint .path-crumb { color: #0366d6; cursor: pointer; }
        .path-hint .path-crumb:hover { text-decoration: underline; }
        .match-highlight { background-color: #fff5b1; border-radius: 2px; }
        
        /* 搜索结果行样式 */
        tr.search-result { cursor: pointer; }
        tr.search-result:hover { background-color: #dbeafe; }
        tr.search-result td.col-name::after { content: ''; }
        tr.search-result:hover td.col-name::after { opacity: 1; }
        .jump-hint { font-size: 11px; color: #0366d6; margin-left: 8px; opacity: 0.7; }

        /* 搜索框 */
        .search-wrapper {
            flex-grow: 1;
            max-width: 350px;
            position: relative;
        }
        .search-box {
            width: 100%;
            padding: 8px 12px 8px 30px;
            border: 1px solid var(--border);
            border-radius: 6px;
            font-size: 14px;
            background: #f6f8fa;
            transition: .2s;
        }
        .search-box:focus {
            background: #fff;
            border-color: var(--primary);
            outline: none;
            box-shadow: 0 0 0 3px rgba(3,102,214,0.1);
        }
        .search-icon {
            position: absolute;
            left: 10px;
            top: 50%;
            transform: translateY(-50%);
            color: #999;
            font-size: 14px;
            pointer-events: none;
        }

        /* 始终可见的筛选栏 */
        .filter-bar {
            padding: 8px 20px;
            background: #f8fafc;
            border-bottom: 1px solid var(--border);
            display: flex;
            align-items: center;
            gap: 12px;
            font-size: 13px;
        }
        .filter-bar-label {
            color: #64748b;
            font-weight: 500;
            flex-shrink: 0;
        }
        .filter-bar-chips {
            display: flex;
            flex-wrap: wrap;
            gap: 6px;
        }
        .filter-bar-chip {
            display: flex;
            align-items: center;
            gap: 4px;
            padding: 4px 10px;
            border: 1px solid #e2e8f0;
            border-radius: 16px;
            font-size: 12px;
            cursor: pointer;
            transition: all 0.15s;
            background: white;
            color: #64748b;
            user-select: none;
        }
        .filter-bar-chip:hover {
            border-color: #cbd5e1;
            background: #f8fafc;
        }
        .filter-bar-chip.selected {
            background: #dbeafe;
            border-color: #93c5fd;
            color: #1d4ed8;
        }
        .filter-bar-chip input {
            display: none;
        }
        .filter-reset-btn {
            background: none;
            border: none;
            color: #0366d6;
            font-size: 12px;
            cursor: pointer;
            padding: 4px 8px;
            border-radius: 4px;
            transition: background 0.15s;
            flex-shrink: 0;
        }
        .filter-reset-btn:hover {
            background: #dbeafe;
        }
        
        /* GitHub 链接 */
        .github-link {
            display: flex;
            align-items: center;
            justify-content: center;
            width: 36px;
            height: 36px;
            border-radius: 6px;
            color: #24292e;
            transition: all 0.15s;
        }
        .github-link:hover {
            background: var(--hover);
            color: var(--primary);
        }
        /* 面包屑 */
        .breadcrumbs { padding: 8px 20px; font-size: 13px; color: #586069; border-bottom: 1px solid var(--border); display: flex; align-items: center; gap: 6px; overflow-x: auto; white-space: nowrap; background: #fff; min-height: 36px; }
        .crumb { cursor: pointer; color: var(--primary); }
        .crumb:hover { text-decoration: underline; }
        .crumb.current { color: #24292e; font-weight: 600; pointer-events: none; }
        .crumb-sep { color: #ccc; font-size: 12px; }

        /* 加载与空状态 */
        .empty-state { padding: 50px; text-align: center; color: #586069; }
        #loading-overlay { position: fixed; top: 0; left: 0; right: 0; bottom: 0; background: rgba(255,255,255,0.8); display: flex; flex-direction: column; align-items: center; justify-content: center; z-index: 100; display: none; }
        .spinner { width: 30px; height: 30px; border: 3px solid #eee; border-top: 3px solid var(--primary); border-radius: 50%; animation: spin 0.8s linear infinite; }
        @keyframes spin { to { transform: rotate(360deg); } }

        /* Toast 通知 */
        .toast {
            position: fixed;
            bottom: 20px;
            right: 20px;
            background: #1e293b;
            color: white;
            padding: 12px 20px;
            border-radius: 8px;
            font-size: 14px;
            z-index: 1000;
            box-shadow: 0 4px 12px rgba(0,0,0,0.15);
            transform: translateY(100px);
            opacity: 0;
            transition: all 0.3s ease;
        }
        .toast.show {
            transform: translateY(0);
            opacity: 1;
        }
        .toast.info { background: #0ea5e9; }
        .toast.success { background: #22c55e; }
        .toast.warning { background: #f59e0b; }

        /* 移动端适配 */
        @media (max-width: 768px) {
            /* 隐藏侧边栏 */
            .sidebar { display: none; }
            
            /* 主视图全屏 */
            .main-view { width: 100%; }
            
            /* 简化头部 */
            .header { 
                padding: 0 12px; 
                height: 50px;
            }
            .header h1 { font-size: 15px; }
            .header-right { gap: 8px; }
            .search-wrapper { 
                max-width: none; 
                flex: 1;
            }
            .search-box { 
                padding: 6px 10px 6px 28px; 
                font-size: 13px;
            }
            .search-icon { font-size: 12px; }
            
            /* 隐藏桌面端按钮 */
            .btn-add-cart { display: none; }
            
            /* 面包屑紧凑 */
            .breadcrumbs { 
                padding: 6px 12px; 
                font-size: 12px;
                min-height: 30px;
            }
            
            /* 筛选器横向滚动 */
            .filter-bar {
                padding: 8px 12px;
                overflow-x: auto;
                -webkit-overflow-scrolling: touch;
            }
            .filter-bar-chips { 
                flex-wrap: nowrap; 
                gap: 6px;
            }
            .filter-bar-chip { 
                padding: 4px 10px; 
                font-size: 11px;
                flex-shrink: 0;
            }
            .filter-bar-label { display: none; }
            
            /* 表格紧凑 */
            .col-size { display: none; }
            .col-check { width: 32px; }
            .col-icon { width: 28px; font-size: 14px; }
            td { padding: 10px 8px; font-size: 13px; }
            th { padding: 8px; font-size: 12px; }
            
            /* 搜索结果路径提示 */
            .path-hint { font-size: 10px; }
            .jump-hint { display: none; }
            
            /* 底部固定操作栏 */
            .mobile-actions {
                display: flex;
                position: fixed;
                bottom: 0;
                left: 0;
                right: 0;
                background: #fff;
                border-top: 1px solid var(--border);
                padding: 10px 16px;
                gap: 10px;
                z-index: 50;
                box-shadow: 0 -2px 10px rgba(0,0,0,0.1);
            }
            .mobile-actions .btn {
                flex: 1;
                justify-content: center;
            }
            
            /* 给内容留出底部空间 */
            .file-list { padding-bottom: 70px; }
        }
        
        /* 桌面端隐藏移动端元素 */
        @media (min-width: 769px) {
            .mobile-actions { display: none; }
        }

        /* === Download Cart 样式 === */
        .download-cart {
            border-top: 1px solid var(--border);
            background: linear-gradient(180deg, #f8fafc 0%, #f1f5f9 100%);
            display: flex;
            flex-direction: column;
            height: 300px;
            transition: height 0.3s ease;
            flex-shrink: 0;
        }
        .download-cart.collapsed {
            height: 48px;
            overflow: hidden;
        }
        .cart-header {
            display: flex;
            align-items: center;
            justify-content: space-between;
            padding: 12px 16px;
            cursor: pointer;
            user-select: none;
            background: #fff;
            border-bottom: 1px solid var(--border);
        }
        .cart-header:hover {
            background: var(--hover);
        }
        .cart-title {
            display: flex;
            align-items: center;
            gap: 8px;
            font-weight: 600;
            font-size: 14px;
            color: #24292e;
        }
        .cart-badge {
            background: var(--primary);
            color: white;
            font-size: 11px;
            font-weight: 600;
            padding: 2px 7px;
            border-radius: 10px;
            min-width: 20px;
            text-align: center;
        }
        .cart-badge.empty {
            background: #94a3b8;
        }
        .cart-toggle {
            font-size: 12px;
            color: #64748b;
            transition: transform 0.2s;
        }
        .download-cart.collapsed .cart-toggle {
            transform: rotate(180deg);
        }
        .cart-body {
            flex-grow: 1;
            overflow-y: auto;
            padding: 8px 0;
        }
        .download-cart.collapsed .cart-body,
        .download-cart.collapsed .cart-footer {
            display: none;
        }
        .cart-item {
            display: flex;
            align-items: center;
            padding: 8px 16px;
            gap: 10px;
            font-size: 13px;
            color: #334155;
            transition: background 0.15s;
        }
        .cart-item:hover {
            background: var(--hover);
        }
        .cart-item-icon {
            flex-shrink: 0;
        }
        .cart-item-name {
            flex-grow: 1;
            overflow: hidden;
            text-overflow: ellipsis;
            white-space: nowrap;
        }
        .cart-item-size {
            font-size: 11px;
            color: #94a3b8;
            flex-shrink: 0;
        }
        .cart-item-remove {
            flex-shrink: 0;
            width: 20px;
            height: 20px;
            border: none;
            background: transparent;
            color: #94a3b8;
            cursor: pointer;
            border-radius: 4px;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 14px;
            transition: all 0.15s;
        }
        .cart-item-remove:hover {
            background: #fee2e2;
            color: #ef4444;
        }
        .cart-empty {
            text-align: center;
            padding: 20px;
            color: #94a3b8;
            font-size: 13px;
        }
        .cart-footer {
            padding: 12px 16px;
            border-top: 1px solid var(--border);
            background: #fff;
            display: flex;
            flex-direction: column;
            gap: 10px;
        }
        .cart-stats {
            font-size: 12px;
            color: #64748b;
            display: flex;
            justify-content: space-between;
        }
        .cart-actions {
            display: flex;
            gap: 8px;
        }
        .btn-cart {
            flex: 1;
            padding: 8px 12px;
            border-radius: 6px;
            font-size: 13px;
            font-weight: 500;
            cursor: pointer;
            transition: all 0.15s;
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 6px;
        }
        .btn-cart-clear {
            background: #f1f5f9;
            border: 1px solid #e2e8f0;
            color: #64748b;
        }
        .btn-cart-clear:hover {
            background: #fee2e2;
            border-color: #fecaca;
            color: #dc2626;
        }
        .btn-cart-download {
            background: linear-gradient(135deg, #2ea44f 0%, #22863a 100%);
            border: none;
            color: white;
        }
        .btn-cart-download:hover {
            background: linear-gradient(135deg, #22863a 0%, #176f2c 100%);
            box-shadow: 0 2px 8px rgba(34, 134, 58, 0.3);
        }
        .btn-cart-download:disabled {
            background: #94d3a2;
            cursor: not-allowed;
            box-shadow: none;
        }
        .btn-add-cart {
            background: #dbeafe;
            color: #1d4ed8;
            border: 1px solid #bfdbfe;
        }
        .btn-add-cart:hover:not(:disabled) {
            background: #bfdbfe;
        }
        .btn-add-cart:disabled {
            background: #f1f5f9;
            color: #94a3b8;
            border-color: #e2e8f0;
            cursor: not-allowed;
        }

        /* 动画 */
        @keyframes cartItemAdd {
            from { opacity: 0; transform: translateX(-10px); }
            to { opacity: 1; transform: translateX(0); }
        }
        .cart-item {
            animation: cartItemAdd 0.2s ease;
        }
    </style>
</head>
<body>
    <div class="sidebar">
        <div class="sidebar-title">
            <a href="https://github.com/vwOvOwv/PKU-Undergraduate-Course" target="_blank" class="github-link" title="View on GitHub" style="margin-right: 10px;">
                <svg height="24" viewBox="0 0 16 16" width="24" fill="currentColor"><path d="M8 0C3.58 0 0 3.58 0 8c0 3.54 2.29 6.53 5.47 7.59.4.07.55-.17.55-.38 0-.19-.01-.82-.01-1.49-2.01.37-2.53-.49-2.69-.94-.09-.23-.48-.94-.82-1.13-.28-.15-.68-.52-.01-.53.63-.01 1.08.58 1.23.82.72 1.21 1.87.87 2.33.66.07-.52.28-.87.51-1.07-1.78-.2-3.64-.89-3.64-3.95 0-.87.31-1.59.82-2.15-.08-.2-.36-1.02.08-2.12 0 0 .67-.21 2.2.82.64-.18 1.32-.27 2-.27.68 0 1.36.09 2 .27 1.53-1.04 2.2-.82 2.2-.82.44 1.1.16 1.92.08 2.12.51.56.82 1.27.82 2.15 0 3.07-1.87 3.75-3.65 3.95.29.25.54.73.54 1.48 0 1.07-.01 1.93-.01 2.2 0 .21.15.46.55.38A8.013 8.013 0 0016 8c0-4.42-3.58-8-8-8z"></path></svg>
            </a>
            Peiyu's Course Zoo
        </div>
        <div class="tree-container" id="folder-tree"></div>
        
        <!-- Download Cart Panel -->
        <div class="download-cart" id="download-cart">
            <div class="cart-header" onclick="toggleCartPanel()">
                <div class="cart-title">
                    <span>🛒</span>
                    <span>Download Cart</span>
                    <span class="cart-badge empty" id="cart-badge">0</span>
                </div>
                <span class="cart-toggle">▼</span>
            </div>
            <div class="cart-body" id="cart-body">
                <div class="cart-empty" id="cart-empty">No items in cart</div>
            </div>
            <div class="cart-footer">
                <div class="cart-stats">
                    <span id="cart-file-count">0 files</span>
                    <span id="cart-total-size">~0 KB</span>
                </div>
                <div class="cart-actions">
                    <button class="btn-cart btn-cart-clear" onclick="clearCart()">🗑 Clear All</button>
                    <button class="btn-cart btn-cart-download" id="cart-download-btn" onclick="downloadCart()" disabled>⬇ Download All</button>
                </div>
            </div>
        </div>
    </div>

    <div class="main-view">
        <div class="header">
            <h1 id="view-title">Course Zoo</h1>
            
            <div class="header-right">
                <div class="search-wrapper" id="search-wrapper">
                    <span class="search-icon">🔍</span>
                    <input type="text" class="search-box" id="search-input" placeholder="Search files..." oninput="handleSearch(this.value)">
                </div>
                <button class="btn btn-add-cart" id="add-cart-btn" onclick="addToCart()" disabled>
                    <span>🛒</span> Add to Cart
                </button>
            </div>
        </div>
        
        <div class="breadcrumbs" id="breadcrumbs"></div>
        
        <!-- 始终可见的筛选栏 -->
        <div class="filter-bar" id="filter-bar">
            <span class="filter-bar-label">Show:</span>
            <div class="filter-bar-chips">
                <label class="filter-bar-chip selected" id="chip-folder">
                    <input type="checkbox" checked onchange="updateFilters()">
                    <span>📁</span> Folder
                </label>
                <label class="filter-bar-chip selected" id="chip-pdf">
                    <input type="checkbox" checked onchange="updateFilters()">
                    <span>📕</span> PDF
                </label>
                <label class="filter-bar-chip selected" id="chip-doc">
                    <input type="checkbox" checked onchange="updateFilters()">
                    <span>📘</span> Word
                </label>
                <label class="filter-bar-chip selected" id="chip-ppt">
                    <input type="checkbox" checked onchange="updateFilters()">
                    <span>🔖</span> PPT
                </label>
                <label class="filter-bar-chip selected" id="chip-list">
                    <input type="checkbox" checked onchange="updateFilters()">
                    <span>📊</span> List
                </label>
                <label class="filter-bar-chip selected" id="chip-md">
                    <input type="checkbox" checked onchange="updateFilters()">
                    <span>📝</span> Markdown
                </label>
                <label class="filter-bar-chip selected" id="chip-code">
                    <input type="checkbox" checked onchange="updateFilters()">
                    <span>💻</span> Code
                </label>
                <label class="filter-bar-chip selected" id="chip-zip">
                    <input type="checkbox" checked onchange="updateFilters()">
                    <span>📦</span> Archive
                </label>
                <label class="filter-bar-chip selected" id="chip-other">
                    <input type="checkbox" checked onchange="updateFilters()">
                    <span>📄</span> Other
                </label>
            </div>
            <button class="filter-reset-btn" id="filter-reset-btn" onclick="resetFilters()" style="display:none">↺ Reset</button>
        </div>

        <div class="file-list">
            <table>
                <thead>
                    <tr>
                        <th class="col-check"><input type="checkbox" id="select-all" onclick="toggleSelectAll()"></th>
                        <th class="col-icon"></th>
                        <th class="col-name">Name</th>
                        <th class="col-size">Size</th>
                    </tr>
                </thead>
                <tbody id="file-table-body"></tbody>
            </table>
            <div id="empty-msg" class="empty-state" style="display:none;">This folder is empty.</div>
        </div>
        
        <!-- 移动端底部操作栏 -->
        <div class="mobile-actions">
            <button class="btn btn-add-cart" id="mobile-add-cart-btn" onclick="addToCart()" disabled>
                <span>🛒</span> Add to Cart (<span id="mobile-select-count">0</span>)
            </button>
            <button class="btn" onclick="showMobileCart()" style="background: var(--primary);">
                📦 Cart (<span id="mobile-cart-count">0</span>)
            </button>
        </div>
    </div>

    <div id="loading-overlay">
        <div id="loading-speed" style="margin-bottom: 15px; font-size: 13px; color: #94a3b8;"></div>
        <div class="spinner"></div>
        <div id="loading-text" style="margin-top: 15px; font-size: 14px; font-weight: 500;">Processing...</div>
        <div id="loading-eta" style="margin-top: 8px; font-size: 12px; color: #64748b;"></div>
        <button id="cancel-download-btn" onclick="cancelDownload()" style="margin-top: 20px; padding: 8px 20px; background: #ef4444; color: white; border: none; border-radius: 6px; cursor: pointer; font-size: 13px; display: none;">Cancel Download</button>
    </div>

    <div id="toast" class="toast"></div>

    <script>
        const rawData = __DATA_JSON__;
        const BASE_URL = "__BASE_URL__";
        
        let currentFolder = null;
        let isSearchMode = false;
        let folderMap = new Map();
        let allFiles = []; // 搜索索引
        let selectedFiles = new Set();
        let cartItems = new Map(); // Download Cart storage

        // === Toast 通知函数 ===
        function showToast(message, type = 'info', duration = 3000) {
            const toast = document.getElementById('toast');
            toast.textContent = message;
            toast.className = 'toast ' + type;
            toast.classList.add('show');
            setTimeout(() => {
                toast.classList.remove('show');
            }, duration);
        }

        // === 文件类型筛选器 ===
        let activeFilters = {
            folder: true,
            pdf: true,
            doc: true,
            ppt: true,
            list: true,
            md: true,
            code: true,
            zip: true,
            other: true
        };


        function toggleFilterMenu(e) {
            if (e) e.stopPropagation();
            const menu = document.getElementById('filter-menu');
            const toggle = document.getElementById('filter-toggle');
            menu.classList.toggle('show');
            toggle.classList.toggle('open', menu.classList.contains('show'));
        }

        function onSearchFocus() {
            // 可以在这里添加搜索聚焦时的行为
        }

        // 筛选类型的显示名称和图标
        const filterLabels = {
            folder: { name: 'Folder', icon: '📁' },
            pdf: { name: 'PDF', icon: '📕' },
            doc: { name: 'Word', icon: '📘' },
            ppt: { name: 'PPT', icon: '🔖' },
            list: { name: 'List', icon: '📊' },
            md: { name: 'Markdown', icon: '📝' },
            code: { name: 'Code', icon: '💻' },
            zip: { name: 'Archive', icon: '📦' },
            other: { name: 'Other', icon: '📄' }
        };
        
        // 获取当前项目列表中存在的文件类型
        function getActiveFileTypes(items) {
            const types = new Set();
            items.forEach(item => {
                if (item.type === 'folder') {
                    types.add('folder');
                } else {
                    const ext = getFileExtension(item.name);
                    if (['pdf'].includes(ext)) types.add('pdf');
                    else if (['doc', 'docx'].includes(ext)) types.add('doc');
                    else if (['ppt', 'pptx'].includes(ext)) types.add('ppt');
                    else if (['xls', 'xlsx', 'csv'].includes(ext)) types.add('list');
                    else if (['py', 'js', 'ts', 'java', 'c', 'cpp', 'h', 'hpp', 'cs', 'go', 'rs', 'rb', 'php', 'swift', 'kt', 'scala', 'sh', 'bash', 'zsh', 'json', 'xml', 'yaml', 'yml', 'html', 'css', 'sql', 'r', 'lua', 'perl', 'asm', 's', 'ipynb'].includes(ext)) types.add('code');
                    else if (['md', 'markdown', 'txt', 'rst'].includes(ext)) types.add('md');
                    else if (['zip', 'rar', '7z', 'tar', 'gz', 'bz2', 'xz'].includes(ext)) types.add('zip');
                    else types.add('other');
                }
            });
            return types;
        }
        
        // 根据当前文件夹内容更新筛选器可见性
        function updateFilterVisibility(items) {
            const activeTypes = getActiveFileTypes(items);
            const chipIds = ['folder', 'pdf', 'doc', 'ppt', 'list', 'md', 'code', 'zip', 'other'];
            
            chipIds.forEach(id => {
                const chip = document.getElementById('chip-' + id);
                if (chip) {
                    if (activeTypes.has(id)) {
                        chip.style.display = 'flex';
                    } else {
                        chip.style.display = 'none';
                    }
                }
            });
        }

        function updateFilters() {
            // 从 chip 元素读取状态
            const chipIds = ['folder', 'pdf', 'doc', 'ppt', 'list', 'md', 'code', 'zip', 'other'];
            chipIds.forEach(id => {
                const chip = document.getElementById('chip-' + id);
                const checkbox = chip.querySelector('input');
                activeFilters[id] = checkbox.checked;
                chip.classList.toggle('selected', checkbox.checked);
            });
            
            // 显示/隐藏 Reset 按钮
            const allChecked = Object.values(activeFilters).every(v => v);
            const resetBtn = document.getElementById('filter-reset-btn');
            resetBtn.style.display = allChecked ? 'none' : 'block';
            
            // 刷新显示
            const searchVal = document.getElementById('search-input').value;
            if (searchVal || isSearchMode) {
                handleSearch(searchVal);
            } else if (currentFolder) {
                // 非搜索模式下也刷新文件列表
                const filteredItems = (currentFolder.children || []).filter(item => matchesFilter(item));
                renderList(filteredItems);
            }
        }

        function resetFilters() {
            const chipIds = ['folder', 'pdf', 'doc', 'ppt', 'list', 'md', 'code', 'zip', 'other'];
            chipIds.forEach(id => {
                const chip = document.getElementById('chip-' + id);
                const checkbox = chip.querySelector('input');
                checkbox.checked = true;
                activeFilters[id] = true;
                chip.classList.add('selected');
            });
            
            document.getElementById('filter-reset-btn').style.display = 'none';
            
            // 刷新显示
            const searchVal = document.getElementById('search-input').value;
            if (searchVal || isSearchMode) {
                handleSearch(searchVal);
            } else if (currentFolder) {
                renderList(currentFolder.children || []);
            }
            
            showToast('Filters reset', 'success');
        }

        function getFileExtension(filename) {
            const ext = filename.split('.').pop().toLowerCase();
            return ext === filename.toLowerCase() ? '' : ext;
        }

        function matchesFilter(item) {
            if (item.type === 'folder') return activeFilters.folder;
            
            const ext = getFileExtension(item.name);
            
            if (['pdf'].includes(ext)) return activeFilters.pdf;
            if (['doc', 'docx'].includes(ext)) return activeFilters.doc;
            if (['ppt', 'pptx'].includes(ext)) return activeFilters.ppt;
            if (['xls', 'xlsx', 'csv'].includes(ext)) return activeFilters.list;
            if (['py', 'js', 'ts', 'java', 'c', 'cpp', 'h', 'hpp', 'cs', 'go', 'rs', 'rb', 'php', 'swift', 'kt', 'scala', 'sh', 'bash', 'zsh', 'json', 'xml', 'yaml', 'yml', 'html', 'css', 'sql', 'r', 'lua', 'perl', 'asm', 's', 'ipynb'].includes(ext)) return activeFilters.code;
            if (['md', 'markdown', 'txt', 'rst'].includes(ext)) return activeFilters.md;
            if (['zip', 'rar', '7z', 'tar', 'gz', 'bz2', 'xz'].includes(ext)) return activeFilters.zip;
            
            return activeFilters.other;
        }

        // 根据文件类型获取图标
        function getFileIcon(item) {
            if (item.type === 'folder') return '📁';
            
            const ext = getFileExtension(item.name);
            
            if (['pdf'].includes(ext)) return '📕';
            if (['doc', 'docx'].includes(ext)) return '📘';
            if (['ppt', 'pptx'].includes(ext)) return '🔖';
            if (['xls', 'xlsx', 'csv'].includes(ext)) return '📊';
            if (['py', 'js', 'ts', 'java', 'c', 'cpp', 'h', 'hpp', 'cs', 'go', 'rs', 'rb', 'php', 'swift', 'kt', 'scala', 'sh', 'bash', 'zsh', 'json', 'xml', 'yaml', 'yml', 'html', 'css', 'sql', 'r', 'lua', 'perl', 'asm', 's', 'rs', 'mlx', 'ipynb'].includes(ext)) return '💻';
            if (['md', 'markdown', 'txt', 'rst'].includes(ext)) return '📝';
            if (['zip', 'rar', '7z', 'tar', 'gz', 'bz2', 'xz'].includes(ext)) return '📦';
            if (['jpg', 'jpeg', 'png', 'gif', 'bmp', 'svg', 'webp', 'ico', 'fig'].includes(ext)) return '🏞️';
            if (['mp4', 'avi', 'mov', 'mkv', 'wmv', 'flv', 'webm'].includes(ext)) return '🎬';
            if (['mp3', 'wav', 'flac', 'aac', 'ogg', 'wma'].includes(ext)) return '🎵';
            
            return '📄';
        }

        // === 辅助函数：格式化文件大小 ===
        function formatSize(sizeKB) {
            if (sizeKB == null || sizeKB === 0) return '-';
            if (sizeKB >= 1024) {
                return (sizeKB / 1024).toFixed(1) + ' MB';
            }
            return sizeKB.toFixed(1) + ' KB';
        }

        // === 1. 初始化索引 (修改版：同时索引文件夹) ===

        function indexData(nodes, parentPath = []) {
            if (!nodes) return;
            nodes.forEach(node => {
                node.parent = parentPath;
                
                // 1. 存入 Map 供 ID 查找
                if (node.type === 'folder') {
                    if (node.id) folderMap.set(node.id, node);
                    // 递归处理子节点
                    if (node.children) indexData(node.children, [...parentPath, node]);
                } 
                
                // 2. 存入搜索索引 (文件和文件夹都加进去)
                // 此时我们要为文件夹动态生成一个 path 字符串，方便搜索显示
                if (!node.relPath && node.parent) {
                    node.relPath = node.parent.map(p => p.name).join('/') + '/' + node.name;
                }
                allFiles.push(node);
            });
        }
        
        // 简单防御：检查数据是否为空
        if (!rawData || rawData.length === 0 || (rawData.length === 2 && !rawData[0].children && !rawData[1].children)) {
            document.getElementById('file-table-body').innerHTML = '<tr><td colspan="4" class="empty-state">⚠️ Error: No course data found.<br>Please check build logs.</td></tr>';
            console.error("Raw Data is empty or invalid", rawData);
        } else {
            indexData(rawData);
            renderTree(rawData, document.getElementById('folder-tree'));
            // 默认打开第一个根目录
            if(rawData[0]) openFolder(rawData[0].id);
        }

        // === 2. 渲染左侧树 ===
        function renderTree(nodes, container, level = 0) {
            nodes.forEach(node => {
                if (node.type !== 'folder') return;

                const div = document.createElement('div');
                div.className = 'tree-item';
                div.dataset.id = node.id;
                div.style.paddingLeft = '15px';
                div.onclick = (e) => { e.stopPropagation(); openFolder(node.id); };

                const showChildren = level < 1; 
                const hasSubs = showChildren && node.children && node.children.some(c => c.type === 'folder');
                
                const toggle = document.createElement('span');
                toggle.className = 'tree-toggle ' + (hasSubs ? '' : 'invisible');
                toggle.innerText = '▶';
                
                // const icon = document.createElement('span');
                // icon.className = 'tree-icon';
                // icon.innerText = '📁';

                const name = document.createElement('span');
                name.innerText = node.name;

                if (level === 0) {
                    name.style.fontWeight = 'bold';
                }

                div.append(toggle, name);
                container.appendChild(div);

                if (hasSubs) {
                    const subDiv = document.createElement('div');
                    subDiv.style.display = 'none';
                    renderTree(node.children, subDiv, level + 1);
                    container.appendChild(subDiv);
                    
                    toggle.onclick = (e) => {
                        e.stopPropagation();
                        const isOpen = subDiv.style.display === 'block';
                        subDiv.style.display = isOpen ? 'none' : 'block';
                        toggle.classList.toggle('open', !isOpen);
                    };
                }
            });
        }

        // === 3. 渲染右侧列表 (修改版：支持搜索跳转) ===
        function renderList(items, isSearch = false) {
            const tbody = document.getElementById('file-table-body');
            tbody.innerHTML = '';
            document.getElementById('empty-msg').style.display = items.length ? 'none' : 'block';
            document.getElementById('select-all').checked = false;
            
            // 更新标题和面包屑
            if (isSearch) {
                document.getElementById('view-title').innerText = `Search Results (${items.length})`;
                document.getElementById('breadcrumbs').innerHTML = '<span class="crumb" onclick="exitSearch()">Back to Folders</span> <span class="crumb-sep">/</span> <span class="crumb current">Search</span>';
            } else {
                let mainTitle = 'Course Zoo'; // 默认兜底
                if (currentFolder) {
                    // 逻辑：如果当前文件夹有父级列表且不为空，取第一个父级（即根节点）；
                    // 否则，说明当前文件夹本身就是根节点 (如 Computer Science)
                    const rootNode = (currentFolder.parent && currentFolder.parent.length > 0) 
                                     ? currentFolder.parent[0] 
                                     : currentFolder;
                    mainTitle = rootNode.name;
                }
                document.getElementById('view-title').innerText = mainTitle;
                updateBreadcrumbs(currentFolder);
            }

            // 排序: 文件夹在前
            if (!isSearch) {
                items.sort((a, b) => {
                    if (a.type === b.type) return a.name.localeCompare(b.name);
                    return a.type === 'folder' ? -1 : 1;
                });
            }

            items.forEach(item => {
                const tr = document.createElement('tr');
                if (isSearch) tr.classList.add('search-result');
                
                // --- 行为逻辑：点击跳转 ---
                // 定义点击行的行为
                const handleRowClick = () => {
                    if (isSearch) {
                        // 搜索模式下：点击即跳转
                        jumpTo(item);
                    } else {
                        // 普通模式下：文件夹进入，文件无动作(或选框)
                        if (item.type === 'folder') openFolder(item.id);
                    }
                };

                // 1. Checkbox 列
                const tdCheck = document.createElement('td');
                tdCheck.className = 'col-check';
                const cb = document.createElement('input');
                cb.type = 'checkbox';
                
                // 文件和文件夹都可以被选中
                cb.checked = selectedFiles.has(item);
                cb.onchange = (e) => {
                    e.stopPropagation();
                    toggleSelection(item, e.target.checked);
                };
                cb.onclick = (e) => e.stopPropagation();
                
                // 点击行的行为
                tr.onclick = (e) => { 
                    if (e.target !== cb) {
                        if (isSearch) {
                            jumpTo(item);
                        } else if (item.type === 'folder') {
                            // 文件夹：双击进入，单击选中
                            openFolder(item.id);
                        } else {
                            // 文件：点击切换选中
                            cb.click();
                        }
                    }
                };
                
                if (cb.checked) tr.classList.add('selected');
                tdCheck.appendChild(cb);

                // 2. Icon 列
                const tdIcon = document.createElement('td');
                tdIcon.className = 'col-icon';
                tdIcon.innerText = getFileIcon(item);

                // 3. Name 列
                const tdName = document.createElement('td');
                tdName.className = 'col-name';
                
                if (isSearch) {
                    // 搜索模式：显示可点击的路径面包屑
                    const searchVal = document.getElementById('search-input').value.toLowerCase();
                    
                    // 高亮匹配的关键词
                    let displayName = item.name;
                    if (searchVal) {
                        const regex = new RegExp(`(${searchVal.replace(/[.*+?^${}()|[\\]\\\\]/g, '\\\\$&')})`, 'gi');
                        displayName = item.name.replace(regex, '<span class="match-highlight">$1</span>');
                    }
                    
                    // 构建可点击的路径面包屑
                    let pathHtml = '';
                    if (item.parent && item.parent.length > 0) {
                        const crumbs = item.parent.map((p, idx) => 
                            `<span class="path-crumb" data-folder-id="${p.id}">${p.name}</span>`
                        ).join(' / ');
                        pathHtml = `<span class="path-hint">${crumbs}</span>`;
                    }
                    
                    tdName.innerHTML = `<div>${displayName}<span class="jump-hint">Go to folder</span></div>${pathHtml}`;
                    
                    // 为路径面包屑添加点击事件
                    setTimeout(() => {
                        tdName.querySelectorAll('.path-crumb').forEach(crumb => {
                            crumb.onclick = (e) => {
                                e.stopPropagation();
                                const folderId = crumb.dataset.folderId;
                                if (folderId) {
                                    document.getElementById('search-input').value = '';
                                    isSearchMode = false;
                                    openFolder(folderId);
                                }
                            };
                        });
                    }, 0);
                } else {
                    tdName.innerText = item.name;
                }

                // 4. Size 列
                const tdSize = document.createElement('td');
                tdSize.className = 'col-size';
                tdSize.innerText = formatSize(item.size);

                tr.append(tdCheck, tdIcon, tdName, tdSize);
                tbody.appendChild(tr);
            });
            
            updateBtnState();
        }

        // === 新增：跳转逻辑 ===
        function jumpTo(item) {
            // 1. 清空搜索状态
            document.getElementById('search-input').value = '';
            isSearchMode = false;

            // 2. 决定要打开哪个文件夹 ID
            let targetFolderId = null;

            if (item.type === 'folder') {
                // 如果搜索到的是文件夹，直接打开它
                targetFolderId = item.id;
            } else {
                // 如果搜索到的是文件，打开它的父文件夹
                if (item.parent && item.parent.length > 0) {
                    targetFolderId = item.parent[item.parent.length - 1].id;
                } else {
                    // 极其罕见的根目录文件情况
                    if (rawData[0]) targetFolderId = rawData[0].id;
                }
            }

            // 3. 执行打开
            if (targetFolderId) {
                openFolder(targetFolderId);
                
                // 可选：如果是文件，可以加一点高亮效果（这里简单实现为滚动到顶部）
                // 实际生产中可能需要用 ID 锚点定位
                document.querySelector('.file-list').scrollTop = 0; 
            }
        }

        function openFolder(id) {
            isSearchMode = false;
            document.getElementById('search-input').value = ''; // 清空搜索
            const node = folderMap.get(id);
            if (!node) return;
            currentFolder = node;
            
            // 左侧高亮
            document.querySelectorAll('.tree-item').forEach(el => el.classList.remove('active'));
            const activeItem = document.querySelector(`.tree-item[data-id="${id}"]`);
            if (activeItem) activeItem.classList.add('active');

            // 更新筛选器可见性
            updateFilterVisibility(node.children || []);
            
            // 应用筛选器
            const filteredItems = (node.children || []).filter(item => matchesFilter(item));
            renderList(filteredItems);
        }

        // === 4. 搜索功能 ===
        function handleSearch(val) {
            val = val.trim().toLowerCase();
            if (!val) {
                if (isSearchMode && currentFolder) openFolder(currentFolder.id);
                return;
            }
            
            isSearchMode = true;
            
            // 1. First find all items that match the search query (ignoring type filters first)
            // This is needed to know which type filters should be visible
            const searchMatches = allFiles.filter(f => f.name.toLowerCase().includes(val));
            
            // 2. Update filter visibility based on search results
            updateFilterVisibility(searchMatches);
            
            // 3. Apply current type filters to render
            const finalResults = searchMatches.filter(f => matchesFilter(f));
            renderList(finalResults, true);
        }

        function exitSearch() {
            document.getElementById('search-input').value = '';
            if (currentFolder) openFolder(currentFolder.id);
            else if (rawData[0]) openFolder(rawData[0].id);
        }

        // === 5. 其他逻辑 ===
        function updateBreadcrumbs(node) {
            const container = document.getElementById('breadcrumbs');
            container.innerHTML = '';
            if (Array.isArray(node) || !node) return;

            const path = [...(node.parent || []), node];
            path.forEach((crumb, index) => {
                if (index > 0) {
                    const sep = document.createElement('span');
                    sep.className = 'crumb-sep';
                    sep.innerText = '/';
                    container.appendChild(sep);
                }
                const span = document.createElement('span');
                span.className = 'crumb' + (index === path.length - 1 ? ' current' : '');
                span.innerText = crumb.name;
                span.onclick = () => openFolder(crumb.id);
                container.appendChild(span);
            });
        }

        function toggleSelection(item, isChecked) {
            if (isChecked) selectedFiles.add(item);
            else selectedFiles.delete(item);
            
            // 更新当前列表的UI
            // (为了性能，这里不重新render整个列表，只更新class)
            // 实际上 renderList 很轻量，重新渲染也可以，或者遍历 DOM
            const checkboxes = document.querySelectorAll('input[type="checkbox"]');
            // ...简化处理，直接重新 render 可能会断开 focus，所以这里只更新按钮
            updateBtnState();
        }
        
        function toggleSelectAll() {
            const selectAllCb = document.getElementById('select-all');
            const isChecked = selectAllCb.checked;
            
            // 获取当前显示的列表数据（应用筛选器）
            let currentItems = isSearchMode ? 
                allFiles.filter(f => f.name.toLowerCase().includes(document.getElementById('search-input').value.toLowerCase()) && matchesFilter(f)) 
                : (currentFolder ? currentFolder.children.filter(item => matchesFilter(item)) : []);

            currentItems.forEach(item => {
                // 文件和文件夹都可以被选中
                if (isChecked) selectedFiles.add(item);
                else selectedFiles.delete(item);
            });
            
            // 只渲染筛选后的列表，不改变筛选状态
            renderList(currentItems, isSearchMode);
            
            // 保持 select-all 复选框状态
            selectAllCb.checked = isChecked;
        }

        function updateBtnState() {
            const addCartBtn = document.getElementById('add-cart-btn');
            const mobileAddCartBtn = document.getElementById('mobile-add-cart-btn');
            const mobileSelectCount = document.getElementById('mobile-select-count');
            const count = selectedFiles.size;
            
            // Desktop button
            addCartBtn.disabled = count === 0;
            addCartBtn.innerHTML = count > 0 ? `<span>🛒</span> Add to Cart (${count})` : `<span>🛒</span> Add to Cart`;
            
            // Mobile button
            if (mobileAddCartBtn) {
                mobileAddCartBtn.disabled = count === 0;
            }
            if (mobileSelectCount) {
                mobileSelectCount.textContent = count;
            }
        }
        
        function updateMobileCartCount() {
            const mobileCartCount = document.getElementById('mobile-cart-count');
            if (mobileCartCount) {
                mobileCartCount.textContent = cartItems.size;
            }
        }
        
        function showMobileCart() {
            // 在移动端显示购物车信息
            if (cartItems.size === 0) {
                alert('Cart is empty');
                return;
            }
            
            let msg = 'Download Cart:\\n\\n';
            let totalSize = 0;
            cartItems.forEach(item => {
                const size = item.size || 0;
                totalSize += size;
                msg += `• ${item.name} (${size >= 1024 ? (size/1024).toFixed(1) + ' MB' : size.toFixed(0) + ' KB'})\\n`;
            });
            msg += `\\nTotal: ${totalSize >= 1024 ? (totalSize/1024).toFixed(1) + ' MB' : totalSize.toFixed(0) + ' KB'}`;
            msg += '\\n\\nDownload all items?';
            
            if (confirm(msg)) {
                downloadCart();
            }
        }

        // === Download Cart Functions ===
        
        function toggleCartPanel() {
            const cart = document.getElementById('download-cart');
            cart.classList.toggle('collapsed');
        }

        // 获取购物车中所有文件夹包含的文件 ID 集合
        function getCartFolderFileIds() {
            const fileIds = new Set();
            cartItems.forEach(item => {
                if (item.type === 'folder') {
                    getAllFilesRecursive(item).forEach(f => fileIds.add(f.id));
                }
            });
            return fileIds;
        }

        // Helper: Check if node A is a descendant of node B (recursive check)
        // Since we don't have parent pointers in all objects or a full tree traversal here easily,
        // we can check if B's recursive file list contains A (if A is file) or A's children.
        // Better yet, we can use the `parent` path attribute if available, or just check ID containment if we had a flat map.
        // Given existing structure, let's stick to checking if item is inside an existing cart folder.
        
        // Expanded Helper: Check if item is inside any folder currently in cart
        function isInsideCartFolder(item, cartFolders) {
            // Check if item's ID is in the file list of any cart folder
            // This works well for files. For folders, we need to check if all its files are in a cart folder?
            // Or simpler: check if the item is a child/descendant of a cart folder.
            // Since we have `getAllFilesRecursive`, we can check if item (or its content) is covered.
            
            for (const folder of cartFolders) {
                if (folder.id === item.id) return true; // Exact match
                
                // Get all IDs inside the cart folder
                const descIds = new Set(getAllFilesRecursive(folder).map(f => f.id));
                
                if (item.type === 'file') {
                    if (descIds.has(item.id)) return true;
                } else {
                    // If item is a folder, check if IT is fully contained in 'folder'
                    // For now, let's simplify: if any file of 'item' is in 'folder', 
                    // and 'item' itself isn't 'folder', it implies 'item' might be a subfolder.
                    // But 'getAllFilesRecursive' returns files. 
                    // Correct approach: recursively check children.
                    
                    // Optimization: We built `getCartFolderFileIds` which returns ALL file IDs in cart folders.
                    // If item is a folder, we check if ALL its files are already in cart folders?
                    // No, "folder in folder" means the parent folder is in cart.
                    // If parent folder is in cart, then ALL its children are implicitly in cart.
                    
                    // Let's rely on the file-based check for simplicity and robustness:
                    // If a folder is "inside" another, all its files are inside.
                    const itemFiles = getAllFilesRecursive(item);
                    if (itemFiles.length > 0 && itemFiles.every(f => descIds.has(f.id))) {
                        return true;
                    }
                    // Handle empty folders? (Edge case, maybe ignore or check ID paths if available)
                }
            }
            return false;
        }

        function addToCart() {
            if (selectedFiles.size === 0) return;
            
            let skippedCount = 0;
            let addedCount = 0;
            let mergedCount = 0;
            
            // Snapshot of current cart items
            const currentCartItems = Array.from(cartItems.values());
            const cartFolders = currentCartItems.filter(i => i.type === 'folder');
            const cartFolderFileIds = getCartFolderFileIds(); // All file IDs currently covered by folders in cart

            selectedFiles.forEach(item => {
                // 1. Check if item is already in cart (direct match)
                if (cartItems.has(item.id)) {
                    skippedCount++;
                    return;
                }

                // 2. Check if item is already covered by a folder in cart
                // (Files inside folders, or subfolders inside folders)
                if (isInsideCartFolder(item, cartFolders)) {
                     skippedCount++;
                     return;
                }
                
                // 3. If adding a folder, check if it "swallows" existing cart items
                // (i.e. we are adding a parent folder, so we should remove its children from cart)
                if (item.type === 'folder') {
                    const newItemFileIds = new Set(getAllFilesRecursive(item).map(f => f.id));
                    const toRemove = [];
                    
                    cartItems.forEach(existing => {
                        // If existing item (file or folder) is completely inside the new item
                        if (existing.type === 'file') {
                            if (newItemFileIds.has(existing.id)) {
                                toRemove.push(existing.id);
                            }
                        } else {
                            // If existing folder is inside new folder
                            // Check if all its files are in new folder
                            const existingFiles = getAllFilesRecursive(existing);
                            if (existingFiles.length > 0 && existingFiles.every(f => newItemFileIds.has(f.id))) {
                                toRemove.push(existing.id);
                            }
                        }
                    });
                    
                    if (toRemove.length > 0) {
                        toRemove.forEach(id => cartItems.delete(id));
                        mergedCount += toRemove.length;
                    }
                }

                // Add the item
                cartItems.set(item.id, item);
                addedCount++;
            });
            
            // Clear selection after adding
            selectedFiles.clear();
            if (currentFolder) {
                const filteredItems = (currentFolder.children || []).filter(item => matchesFilter(item));
                renderList(filteredItems, false);
            }
            
            updateCartUI();
            saveCartToStorage();
            
            // Expand cart panel if collapsed
            const cart = document.getElementById('download-cart');
            if (cart.classList.contains('collapsed')) {
                cart.classList.remove('collapsed');
            }
            
            // 显示提示信息
            let msg = `Added ${addedCount} item${addedCount !== 1 ? 's' : ''}`;
            let msgParts = [];
            if (skippedCount > 0) msgParts.push(`${skippedCount} duplicate${skippedCount !== 1 ? 's' : ''} skipped`);
            if (mergedCount > 0) msgParts.push(`${mergedCount} existing item${mergedCount !== 1 ? 's' : ''} merged`);
            if (msgParts.length > 0) msg += ' (' + msgParts.join(', ') + ')';
            
            if (addedCount > 0 || skippedCount > 0 || mergedCount > 0) {
                showToast(msg, (skippedCount > 0 || mergedCount > 0) ? 'info' : 'success');
            }
        }

        function removeFromCart(itemId) {
            const item = cartItems.get(itemId);
            const itemName = item ? item.name : 'Item';
            cartItems.delete(itemId);
            updateCartUI();
            saveCartToStorage();
            showToast(`Removed "${itemName}" from cart`, 'info');
        }

        function clearCart() {
            if (cartItems.size === 0) return;
            if (!confirm('Clear all items from the cart?')) return;
            const count = cartItems.size;
            cartItems.clear();
            updateCartUI();
            saveCartToStorage();
            showToast(`Cleared ${count} item${count !== 1 ? 's' : ''} from cart`, 'warning');
        }

        function getAllFilesRecursive(node) {
            let files = [];
            if (node.type === 'file') {
                files.push(node);
            } else if (node.type === 'folder' && node.children) {
                node.children.forEach(child => {
                    files = files.concat(getAllFilesRecursive(child));
                });
            }
            return files;
        }

        function calculateTotalSize() {
            let totalKB = 0;
            cartItems.forEach(item => {
                if (item.type === 'file') {
                    totalKB += item.size || 0;
                } else {
                    // Recursively calculate folder size
                    const files = getAllFilesRecursive(item);
                    files.forEach(f => totalKB += f.size || 0);
                }
            });
            return totalKB;
        }

        function countFiles() {
            let count = 0;
            cartItems.forEach(item => {
                if (item.type === 'file') {
                    count++;
                } else {
                    count += getAllFilesRecursive(item).length;
                }
            });
            return count;
        }

        function updateCartUI() {
            const body = document.getElementById('cart-body');
            const badge = document.getElementById('cart-badge');
            const emptyMsg = document.getElementById('cart-empty');
            const downloadBtn = document.getElementById('cart-download-btn');
            const fileCountEl = document.getElementById('cart-file-count');
            const totalSizeEl = document.getElementById('cart-total-size');
            
            // Update badge
            const itemCount = cartItems.size;
            badge.textContent = itemCount;
            badge.classList.toggle('empty', itemCount === 0);
            
            // Update mobile cart count
            updateMobileCartCount();
            
            // Update stats
            const fileCount = countFiles();
            const totalSize = calculateTotalSize();
            fileCountEl.textContent = fileCount === 1 ? '1 file' : `${fileCount} files`;
            totalSizeEl.textContent = totalSize >= 1024 ? `~${(totalSize / 1024).toFixed(1)} MB` : `~${totalSize.toFixed(0)} KB`;
            
            // Update download button
            downloadBtn.disabled = itemCount === 0;
            
            // Clear and rebuild body
            body.innerHTML = '';
            
            if (itemCount === 0) {
                body.innerHTML = '<div class="cart-empty">No items in cart</div>';
                return;
            }
            
            cartItems.forEach((item, id) => {
                const div = document.createElement('div');
                div.className = 'cart-item';
                
                const icon = document.createElement('span');
                icon.className = 'cart-item-icon';
                icon.textContent = item.type === 'folder' ? '📁' : getFileIcon(item);
                
                const infoDiv = document.createElement('div');
                infoDiv.style.flexGrow = '1';
                infoDiv.style.display = 'flex';
                infoDiv.style.flexDirection = 'column';
                infoDiv.style.overflow = 'hidden';
                
                const name = document.createElement('span');
                name.className = 'cart-item-name';
                name.textContent = item.name;
                name.title = item.name;
                
                const course = document.createElement('span');
                course.style.fontSize = '11px';
                course.style.color = '#94a3b8';
                course.textContent = getCourseName(item);
                
                infoDiv.appendChild(name);
                infoDiv.appendChild(course);
                
                const size = document.createElement('span');
                size.className = 'cart-item-size';
                if (item.type === 'file') {
                    size.textContent = formatSize(item.size);
                } else {
                    const folderFiles = getAllFilesRecursive(item);
                    // Use item.size if available, otherwise calculate from children
                    const folderSize = item.size || 0;
                    size.textContent = `${folderFiles.length} files, ${formatSize(folderSize)}`;
                }
                
                const removeBtn = document.createElement('button');
                removeBtn.className = 'cart-item-remove';
                removeBtn.textContent = '✕';
                removeBtn.title = 'Remove from cart';
                removeBtn.onclick = (e) => {
                    e.stopPropagation();
                    removeFromCart(id);
                };
                
                div.appendChild(icon);
                div.appendChild(infoDiv);
                div.appendChild(size);
                div.appendChild(removeBtn);
                
                body.appendChild(div);
            });
        }
        
        function getCourseName(item) {
            // Path structure: Branch/CourseName/...
            // e.g. ComputerScience/DataStructure/notes.pdf
            const parts = (item.urlPath || '').split('/');
            if (parts.length >= 2) {
                return parts[1];
            }
            return '';
        }

        // Global abort controller for download cancellation
        let downloadAbortController = null;
        
        function cancelDownload() {
            if (downloadAbortController) {
                downloadAbortController.abort();
            }
        }
        
        function formatETA(seconds) {
            if (!isFinite(seconds) || seconds <= 0) return 'Calculating...';
            if (seconds < 60) return `~${Math.ceil(seconds)}s remaining`;
            if (seconds < 3600) {
                const mins = Math.floor(seconds / 60);
                const secs = Math.ceil(seconds % 60);
                return `~${mins}m ${secs}s remaining`;
            }
            const hours = Math.floor(seconds / 3600);
            const mins = Math.ceil((seconds % 3600) / 60);
            return `~${hours}h ${mins}m remaining`;
        }

        async function downloadCart() {
            if (cartItems.size === 0) return;
            
            const overlay = document.getElementById('loading-overlay');
            const text = document.getElementById('loading-text');
            const speedEl = document.getElementById('loading-speed');
            const etaEl = document.getElementById('loading-eta');
            const cancelBtn = document.getElementById('cancel-download-btn');
            
            overlay.style.display = 'flex';
            speedEl.innerText = '';
            etaEl.innerText = 'Calculating...';
            cancelBtn.style.display = 'inline-block';
            
            // Create abort controller
            downloadAbortController = new AbortController();
            const signal = downloadAbortController.signal;
            
            // Collect all files (flatten folders)
            let allCartFiles = [];
            cartItems.forEach(item => {
                if (item.type === 'file') {
                    allCartFiles.push({ file: item, path: item.name });
                } else {
                    // For folders, preserve directory structure
                    const files = getAllFilesRecursive(item);
                    files.forEach(f => {
                        // Build relative path within the folder
                        const folderPath = item.name;
                        const filePath = f.relPath.replace(item.relPath + '/', '').replace(item.relPath, '');
                        allCartFiles.push({ file: f, path: folderPath + '/' + (filePath || f.name) });
                    });
                }
            });
            
            // Calculate total expected size (in KB from data, convert to bytes)
            const totalExpectedBytes = allCartFiles.reduce((sum, item) => sum + (item.file.size || 0) * 1024, 0);
            
            const zip = new JSZip();
            let count = 0;
            let totalBytesDownloaded = 0;
            let startTime = Date.now();
            
            function formatSpeed(bytesPerSec) {
                if (bytesPerSec >= 1024 * 1024) {
                    return (bytesPerSec / (1024 * 1024)).toFixed(1) + ' MB/s';
                } else if (bytesPerSec >= 1024) {
                    return (bytesPerSec / 1024).toFixed(1) + ' KB/s';
                }
                return bytesPerSec.toFixed(0) + ' B/s';
            }
            
            // Real-time speed tracking
            let lastBytes = 0;
            let lastTime = Date.now();
            let currentSpeed = 0;
            
            const speedInterval = setInterval(() => {
                const now = Date.now();
                const diffTime = (now - lastTime) / 1000;
                if (diffTime >= 0.5) {
                    const diffBytes = totalBytesDownloaded - lastBytes;
                    currentSpeed = diffBytes / diffTime;
                    speedEl.innerText = formatSpeed(currentSpeed);
                    
                    // Calculate ETA
                    const bytesRemaining = totalExpectedBytes - totalBytesDownloaded;
                    const etaSeconds = currentSpeed > 0 ? bytesRemaining / currentSpeed : 0;
                    etaEl.innerText = formatETA(etaSeconds);
                    
                    lastBytes = totalBytesDownloaded;
                    lastTime = now;
                }
            }, 500);
            
            let interrupted = false;
            
            try {
                const SIZE_THRESHOLD_KB = 20 * 1024; // 20MB threshold
                const GITHUB_RAW_BASE = 'https://raw.githubusercontent.com/vwOvOwv/PKU-Undergraduate-Course';
                
                for (const {file, path} of allCartFiles) {
                    // Check if aborted
                    if (signal.aborted) {
                        interrupted = true;
                        break;
                    }
                    
                    text.innerText = `Downloading ${count + 1}/${allCartFiles.length}: ${file.name}`;
                    
                    const parts = file.urlPath.split('/');
                    const branch = parts[0];
                    const filePathParts = parts.slice(1);
                    const encodedPath = filePathParts.map(p => encodeURIComponent(p)).join('/');
                    
                    // Decide which CDN to use based on file size
                    const useRaw = file.size > SIZE_THRESHOLD_KB;
                    let url = useRaw 
                        ? `${GITHUB_RAW_BASE}/${branch}/${encodedPath}`
                        : `${BASE_URL}@${branch}/${encodedPath}`;
                    
                    let res = await fetch(url, { signal });
                    
                    // Fallback to GitHub Raw if jsDelivr fails
                    if (!res.ok && !useRaw) {
                        console.warn(`jsDelivr failed for ${file.name}, trying GitHub Raw...`);
                        url = `${GITHUB_RAW_BASE}/${branch}/${encodedPath}`;
                        res = await fetch(url, { signal });
                    }
                    
                    if (!res.ok) {
                        console.error('Download failed URL:', url);
                        throw new Error(`HTTP ${res.status} for ${file.name}`);
                    }
                    
                    // Use stream to track real-time progress
                    const reader = res.body.getReader();
                    const chunks = [];
                    while (true) {
                        const {done, value} = await reader.read();
                        if (done) break;
                        if (signal.aborted) {
                            interrupted = true;
                            break;
                        }
                        chunks.push(value);
                        totalBytesDownloaded += value.length;
                    }
                    
                    if (interrupted) break;
                    
                    const blob = new Blob(chunks);
                    zip.file(path, blob);
                    count++;
                }
                
                clearInterval(speedInterval);
                cancelBtn.style.display = 'none';
                
                if (interrupted) {
                    // Offer partial download
                    if (count > 0) {
                        etaEl.innerText = '';
                        speedEl.innerText = '';
                        text.innerText = `Download cancelled. Saving ${count} downloaded files...`;
                        const content = await zip.generateAsync({type:'blob'});
                        saveAs(content, 'PKU_undergrad_course_materials_partial_download.zip');
                        showToast(`Partial download saved (${count}/${allCartFiles.length} files)`, 'warning');
                    } else {
                        showToast('Download cancelled', 'warning');
                    }
                } else {
                    speedEl.innerText = '';
                    etaEl.innerText = '';
                    text.innerText = 'Zipping...';
                    const content = await zip.generateAsync({type:'blob'});
                    saveAs(content, 'PKU_undergrad_course_materials.zip');
                }
                
            } catch (err) {
                clearInterval(speedInterval);
                cancelBtn.style.display = 'none';
                
                if (err.name === 'AbortError') {
                    // User cancelled
                    if (count > 0) {
                        text.innerText = `Cancelled. Saving ${count} downloaded files...`;
                        const content = await zip.generateAsync({type:'blob'});
                        saveAs(content, 'PKU_undergrad_course_materials_partial_download.zip');
                        showToast(`Partial download saved (${count}/${allCartFiles.length} files)`, 'warning');
                    } else {
                        showToast('Download cancelled', 'warning');
                    }
                } else {
                    // Other error - also offer partial download
                    if (count > 0) {
                        const savePartial = confirm(`Download failed: ${err.message}\n\n${count} files were already downloaded. Save partial download?`);
                        if (savePartial) {
                            text.innerText = `Saving ${count} downloaded files...`;
                            const content = await zip.generateAsync({type:'blob'});
                            saveAs(content, 'PKU_undergrad_course_materials_partial_download.zip');
                        }
                    } else {
                        alert('Download failed: ' + err.message);
                    }
                }
            } finally {
                overlay.style.display = 'none';
                downloadAbortController = null;
            }
        }

        // === LocalStorage Persistence ===
        
        function saveCartToStorage() {
            try {
                const ids = Array.from(cartItems.keys());
                localStorage.setItem('downloadCart', JSON.stringify(ids));
            } catch (e) {
                console.warn('Failed to save cart to localStorage:', e);
            }
        }

        function loadCartFromStorage() {
            try {
                const saved = localStorage.getItem('downloadCart');
                if (!saved) return;
                
                const ids = JSON.parse(saved);
                ids.forEach(id => {
                    // Find item by ID in allFiles
                    const item = allFiles.find(f => f.id === id);
                    if (item) {
                        cartItems.set(id, item);
                    }
                });
                updateCartUI();
            } catch (e) {
                console.warn('Failed to load cart from localStorage:', e);
            }
        }

        // Load cart on page load
        loadCartFromStorage();

        // === 6. 下载逻辑 ===
        async function downloadSelected() {
            if (selectedFiles.size === 0) return;
            
            const overlay = document.getElementById('loading-overlay');
            const text = document.getElementById('loading-text');
            overlay.style.display = 'flex';
            
            const zip = new JSZip();
            let count = 0;
            
            try {
                for (const file of selectedFiles) {
                    text.innerText = `Downloading ${count + 1}/${selectedFiles.size}: ${file.name}`;
                    
                    // URL Encode
                    const parts = file.urlPath.split('/');
                    const encodedPath = parts.map(p => encodeURIComponent(p)).join('/');
                    const url = `${BASE_URL}/${encodedPath}`;
                    
                    const res = await fetch(url);
                    if (!res.ok) throw new Error(`HTTP ${res.status}`);
                    zip.file(file.name, await res.blob());
                    count++;
                }
                
                text.innerText = "Zipping...";
                const content = await zip.generateAsync({type:"blob"});
                saveAs(content, "PKU_Materials.zip");
                
            } catch (err) {
                alert("Download failed: " + err.message);
            } finally {
                overlay.style.display = 'none';
                selectedFiles.clear(); // 可选：下载后是否清空
                updateBtnState();
            }
        }
    </script>
</body>
</html>
"""

# 生成 HTML
final_html = html_template.replace("__DATA_JSON__", json.dumps(full_data))
final_html = final_html.replace("__BASE_URL__", BASE_URL)

with open("docs/index.html", "w", encoding="utf-8") as f:
    f.write(final_html)

print("✅ Index generated successfully with Search & Safe IDs.")