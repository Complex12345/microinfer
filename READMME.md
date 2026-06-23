# MicroInfer

Status: Work in progress 

Microinfer is my own inference engine I am building from scratch in C++26. 
The goal of this project is to help me better understand the internals of a llm and the inference pipeline.


The project currently uses TinyLlama-1.1B-Chat-v1.0.Q4_K_M.gguf as the model for inference.



## Current Progress

- GGUF header parsing
- GGUF metadata parsing

## Next Steps

- GGUF Tensor info parsing
- Tensor operation implmentation
- Forward pass implmentation
- Token generation
- Optimizations and cuda support


## Building

```bash
git clone https://github.com/yourname/microinfer.git
cd microinfer

wget -P model https://huggingface.co/TheBloke/TinyLlama-1.1B-Chat-v1.0-GGUF/resolve/main/tinyllama-1.1b-chat-v1.0.Q4_K_M.gguf

cmake -B build
cmake --build build
cd build
./microinfer
```


## Objectives of the project
- Learn the format of GGUF files and how to parse them
- implement Tensor operations from scratch
- Implement inference pipeline from scratch
- Generate text using tinyllama




## Example Output of Current Progress

```text
$ ./microinfer 

GGUF contents:
version: 3
Tensor count: 201
Metadata kv count: 23
general.architecture: llama
general.name: tinyllama_tinyllama-1.1b-chat-v1.0
llama.context_length: 2048
llama.embedding_length: 2048
llama.block_count: 22
llama.feed_forward_length: 5632
llama.rope.dimension_count: 64
llama.attention.head_count: 32
llama.attention.head_count_kv: 4
llama.attention.layer_norm_rms_epsilon: 1e-05
llama.rope.freq_base: 10000
general.file_type: 15
tokenizer.ggml.model: llama
tokenizer.ggml.tokens: [<unk> <s> </s> <0x00> <0x01> <0x02> <0x03> <0x04> <0x05> <0x06> <0x07> <0x08> <0x09> <0x0A> <0x0B> ...
```
