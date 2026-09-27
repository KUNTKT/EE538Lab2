"""Student task: implement the inference function using the vLLM API."""
import argparse
import json
from pathlib import Path
import vllm
from vllm import LLM, SamplingParams
from generation_utils import save_response, save_settings


def generate(model_id, config, prompt):
    # TODO 1: Construct LLM with model_id, configured dtype and vllm settings.
    # TODO 2: Get its tokenizer and apply the chat template to one user message.
    # TODO 3: Build SamplingParams with temperature and max_tokens from config.
    # TODO 4: Call llm.generate and return outputs[0].outputs[0].text.
    raise NotImplementedError('Complete the four steps above.')


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--model', default='qwen3_4b')
    parser.add_argument('--config', default='model_config.json')
    parser.add_argument('--prompt', default='prompts/improved.txt')
    args = parser.parse_args()
    config = json.loads(Path(args.config).read_text())
    model_id = config['models'][args.model]
    prompt = Path(args.prompt).read_text(encoding='utf-8')
    raw = generate(model_id, config, prompt)
    folder = Path('generated') / 'vllm' / args.model
    save_settings(folder, config, model_id, 'vllm', {'vllm': vllm.__version__})
    save_response(folder, Path(args.prompt).stem, prompt, raw)


if __name__ == '__main__':
    main()
