#/bin/bash
echo "Compiling EFI:Hello_World"
echo "gcc -I /home/lalitssrg/gnu-efi-3.0.14/inc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args -c hello_world.c -o hello_world.o"
gcc -I /home/lalitssrg/gnu-efi-3.0.14/inc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args -c hello_world.c -o hello_world.o
echo "Linking EFI:Hello_World"
echo "ld -shared -Bsymbolic -L/home/lalitssrg/gnu-efi-3.0.14/x86_64/lib -L/home/lalitssrg/gnu-efi-3.0.14/x86_64/gnuefi -T/home/lalitssrg/gnu-efi-3.0.14/gnuefi/elf_x86_64_efi.lds /home/lalitssrg/gnu-efi-3.0.14/x86_64/gnuefi/crt0-efi-x86_64.o hello_world.o -o hello_world.so -lgnuefi -lefi"
ld -shared -Bsymbolic -L/home/lalitssrg/gnu-efi-3.0.14/x86_64/lib -L/home/lalitssrg/gnu-efi-3.0.14/x86_64/gnuefi -T/home/lalitssrg/gnu-efi-3.0.14/gnuefi/elf_x86_64_efi.lds /home/lalitssrg/gnu-efi-3.0.14/x86_64/gnuefi/crt0-efi-x86_64.o hello_world.o -o hello_world.so -lgnuefi -lefi
echo "Dumping EFI executable
echo "objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 hello_world.so hello_world.efi"
objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 hello_world.so hello_world.efi
