cmake_minimum_required(VERSION 3.0)
project(BlocksRuntime)

add_library(BlocksRuntime SHARED
        Block.h
        Block_private.h
        data.c
        runtime.c
)
