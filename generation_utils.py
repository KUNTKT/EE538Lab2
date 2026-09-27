"""Small file helpers shared by the two inference examples."""
import json
import re
from pathlib import Path


def save_settings(folder, config, model_id, backend, versions):
    folder.mkdir(parents=True, exist_ok=True)
    settings = dict(model_id=model_id, backend=backend, versions=versions,
                    generation=config['generation'])
    if backend == 'vllm':
        settings['vllm'] = config['vllm']
    (folder / 'settings.json').write_text(json.dumps(settings, indent=2))


def save_response(folder, name, prompt, raw, text_only=False):
    (folder / f'{name}.prompt.txt').write_text(prompt, encoding='utf-8')
    (folder / f'{name}.raw.txt').write_text(raw, encoding='utf-8')
    if not text_only:
        block = re.search(r'```[^\n]*\n(.*?)```', raw, re.S)
        code = block.group(1) if block else raw
        (folder / f'{name}.cpp').write_text(code.strip() + '\n', encoding='utf-8')
    print(f'Saved {folder / name}')
