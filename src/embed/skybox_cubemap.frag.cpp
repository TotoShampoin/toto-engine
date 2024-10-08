#include <array>
#include <stdint.h>
const std::array<char, 407> EMBED_SKYBOX_CUBEMAP_FRAG = {
    0x23, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x20, 0x33, 0x33, 0x30, 0x20, 0x63, 0x6F, 0x72, 
    0x65, 0x0A, 0x6F, 0x75, 0x74, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x66, 0x5F, 0x63, 0x6F, 0x6C, 
    0x6F, 0x72, 0x3B, 0x0A, 0x69, 0x6E, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 0x76, 0x5F, 0x70, 0x6F, 
    0x73, 0x69, 0x74, 0x69, 0x6F, 0x6E, 0x3B, 0x0A, 0x0A, 0x75, 0x6E, 0x69, 0x66, 0x6F, 0x72, 0x6D, 
    0x20, 0x73, 0x61, 0x6D, 0x70, 0x6C, 0x65, 0x72, 0x32, 0x44, 0x20, 0x75, 0x5F, 0x68, 0x64, 0x72, 
    0x69, 0x3B, 0x0A, 0x0A, 0x63, 0x6F, 0x6E, 0x73, 0x74, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x69, 
    0x6E, 0x76, 0x41, 0x74, 0x61, 0x6E, 0x20, 0x3D, 0x20, 0x76, 0x65, 0x63, 0x32, 0x28, 0x30, 0x2E, 
    0x31, 0x35, 0x39, 0x31, 0x2C, 0x20, 0x30, 0x2E, 0x33, 0x31, 0x38, 0x33, 0x29, 0x3B, 0x0A, 0x76, 
    0x65, 0x63, 0x32, 0x20, 0x73, 0x61, 0x6D, 0x70, 0x6C, 0x65, 0x53, 0x70, 0x68, 0x65, 0x72, 0x69, 
    0x63, 0x61, 0x6C, 0x4D, 0x61, 0x70, 0x28, 0x76, 0x65, 0x63, 0x33, 0x20, 0x76, 0x29, 0x20, 0x7B, 
    0x0A, 0x20, 0x20, 0x20, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x75, 0x76, 0x20, 0x3D, 0x20, 0x76, 
    0x65, 0x63, 0x32, 0x28, 0x61, 0x74, 0x61, 0x6E, 0x28, 0x76, 0x2E, 0x7A, 0x2C, 0x20, 0x76, 0x2E, 
    0x78, 0x29, 0x2C, 0x20, 0x61, 0x73, 0x69, 0x6E, 0x28, 0x76, 0x2E, 0x79, 0x29, 0x29, 0x3B, 0x0A, 
    0x20, 0x20, 0x20, 0x20, 0x75, 0x76, 0x20, 0x2A, 0x3D, 0x20, 0x69, 0x6E, 0x76, 0x41, 0x74, 0x61, 
    0x6E, 0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x75, 0x76, 0x20, 0x2B, 0x3D, 0x20, 0x30, 0x2E, 0x35, 
    0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6E, 0x20, 0x75, 0x76, 0x3B, 
    0x0A, 0x7D, 0x0A, 0x0A, 0x76, 0x6F, 0x69, 0x64, 0x20, 0x6D, 0x61, 0x69, 0x6E, 0x28, 0x29, 0x20, 
    0x7B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x75, 0x76, 0x20, 0x3D, 0x20, 
    0x73, 0x61, 0x6D, 0x70, 0x6C, 0x65, 0x53, 0x70, 0x68, 0x65, 0x72, 0x69, 0x63, 0x61, 0x6C, 0x4D, 
    0x61, 0x70, 0x28, 0x6E, 0x6F, 0x72, 0x6D, 0x61, 0x6C, 0x69, 0x7A, 0x65, 0x28, 0x76, 0x5F, 0x70, 
    0x6F, 0x73, 0x69, 0x74, 0x69, 0x6F, 0x6E, 0x29, 0x29, 0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x76, 
    0x65, 0x63, 0x33, 0x20, 0x63, 0x6F, 0x6C, 0x6F, 0x72, 0x20, 0x3D, 0x20, 0x74, 0x65, 0x78, 0x74, 
    0x75, 0x72, 0x65, 0x28, 0x75, 0x5F, 0x68, 0x64, 0x72, 0x69, 0x2C, 0x20, 0x75, 0x76, 0x29, 0x2E, 
    0x72, 0x67, 0x62, 0x3B, 0x0A, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x66, 0x5F, 0x63, 0x6F, 0x6C, 0x6F, 
    0x72, 0x20, 0x3D, 0x20, 0x76, 0x65, 0x63, 0x34, 0x28, 0x63, 0x6F, 0x6C, 0x6F, 0x72, 0x2C, 0x20, 
    0x31, 0x2E, 0x30, 0x29, 0x3B, 0x0A, 0x7D, 
};
