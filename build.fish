set -U fish_user_paths $fish_user_paths /opt/devkitpro/tools/bin
set -U fish_user_paths $fish_user_paths /opt/devkitpro/devkitARM/bin
set -Ux DEVKITARM /opt/devkitpro/devkitARM
set -Ux DEVKITPRO /opt/devkitpro
set -Ux LIBGBA /opt/devkitpro/libgba

make
