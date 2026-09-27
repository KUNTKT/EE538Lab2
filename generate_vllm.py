"""Student task: implement the inference function using the vLLM API."""
import argparse
import json
from pathlib import Path
import vllm
from vllm import LLM, SamplingParams
from generation_utils import save_response, save_settings


def generate(model_id, config, prompt):
    settings = config['generation']
    # TODO 1: Construct LLM with model_id, configured dtype and vllm settings.
    llm = LLM(model=model_id, dtype=settings['dtype'],
              max_model_len=config['vllm']['max_model_len'],
              gpu_memory_utilization=config['vllm']['gpu_memory_utilization'])
    # TODO 2: Get its tokenizer and apply the chat template to one user message.
    tokenizer = llm.get_tokenizer()
    messages = [{'role': 'user', 'content': prompt}]
    text = tokenizer.apply_chat_template(
        messages, tokenize=False, add_generation_prompt=True)
    # TODO 3: Build SamplingParams with temperature and max_tokens from config.
    sampling = SamplingParams(temperature=settings['temperature'],
                              max_tokens=settings['max_new_tokens'])
    # TODO 4: Call llm.generate and return outputs[0].outputs[0].text.
    outputs = llm.generate([text], sampling)
    return outputs[0].outputs[0].text


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
