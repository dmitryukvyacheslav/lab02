project('foxes-and-rabbits', 'cpp')

imgui_proj = subproject('imgui')
imgui_dep = imgui_proj.get_variable('imgui_dep')


deps = []
deps += dependency('sdl2')
deps += dependency('libgvc')
deps += dependency('opengl')
deps += imgui_dep

executable(
    'trevis-bin', \
    'src/main.cpp', \
    'src/rb.cpp', \
    dependencies: deps, \
    include_directories: imgui_proj.get_variable('include_dirs')
)