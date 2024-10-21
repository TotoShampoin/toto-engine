# Toto Engine

A simple rendering engine that uses OpenGL.

## TODO

- [x] Framebuffers, with multiframes rendering
  - [x] Deferred rendering
- [x] HDRI and IBL
  - [x] Skybox
  - [x] Irradiance map
  - [x] Specular IBL
- [ ] Normal mapping
- [ ] Refactor things to a `Drawable` class
  - A drawable has 2 draw functions, for forward and deferred rendering
  - Either could be disabled
- [ ] Line objects
  - [ ] Line shapes (arrows, gizmo)
  - [ ] Wireframes
- [ ] Cull face options
- [ ] Render list, where meshes are added to be rendered
  - [ ] Render ordering
  - [ ] Occlusion culling
  - [ ] Camera culling
- [ ] Model loading
  - [ ] OBJ/MTL
  - [ ] GLTF
- [ ] Bone animations
- [ ] Code documentation
