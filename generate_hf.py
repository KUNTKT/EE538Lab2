"""Provided example: call one configured model using Hugging Face."""
import argparse
import json
from pathlib import Path
import torch
import transformers
from transformers import AutoModelForCausalLM, AutoTokenizer
from generation_utils import save_response, save_settings

parser = argparse.ArgumentParser()
parser.add_argument('--model', default='qwen3_4b')
parser.add_argument('--config', default='model_config.json')
parser.add_argument('--prompts', nargs='+',
                    default=['prompts/basic.txt', 'prompts/improved.txt'])
parser.add_argument('--text-only', action='store_true')
args = parser.parse_args()
config = json.loads(Path(args.config).read_text())
model_id = config['models'][args.model]
settings = config['generation']
tokenizer = AutoTokenizer.from_pretrained(model_id)
model = AutoModelForCausalLM.from_pretrained(
    model_id, torch_dtype=getattr(torch, settings['dtype']), device_map='auto')
model.eval()
folder = Path('generated') / 'hf' / args.model
save_settings(folder, config, model_id, 'hf',
              {'torch': torch.__version__, 'transformers': transformers.__version__})

for filename in args.prompts:
    prompt = Path(filename).read_text(encoding='utf-8')
    messages = [{'role': 'user', 'content': prompt}]
    text = tokenizer.apply_chat_template(
        messages, tokenize=False, add_generation_prompt=True)
    inputs = tokenizer(text, return_tensors='pt',
                       add_special_tokens=False).to(model.device)
    options = dict(max_new_tokens=settings['max_new_tokens'],
                   do_sample=settings['temperature'] > 0,
                   pad_token_id=tokenizer.eos_token_id)
    if settings['temperature'] > 0:
        options['temperature'] = settings['temperature']
    with torch.inference_mode():
        output = model.generate(**inputs, **options)
    raw = tokenizer.decode(output[0, inputs['input_ids'].shape[1]:],
                           skip_special_tokens=True)
    save_response(folder, Path(filename).stem, prompt, raw, args.text_only)
