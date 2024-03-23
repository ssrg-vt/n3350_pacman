## n3350_pacman

![PACMAN](https://github.com/ssrg-vt/n3350_pacman/blob/main/readme_header.png)

#### Introduction
This is the collection of documentation, results and uCode Patches to replicate Pointer Authentication Code Case study on Intel N3350 / Goldmont Plus Architecture
- This work builds upon PACMAN case study presented in Woot'23 publication and Blackhat'22 by Pietro Borrello et.al, which is first known demonstration of PAC code on x86-64.
- uCode match&patch and tracing mechanism is inspired by authors published work [here](https://github.com/pietroborrello/CustomProcessingUnit)
- Intel Atom N3350 (Glodmont) is used CPU of choice to implement of Pointer Authentication Codes(PAC) on x86_64. BMAX_B1 Plus board as testbed.

##### Overview of n3350_pacman repo
- **Makefile** : Makefile to setup/make all the experiments *efi.
- **efi_executables** : Contains *.efi experiment executable, to be copied over to UEFI boot media.
- **hello_world_efi** : Sanity efi compile & link test. Contains few flavors of "Hello World!" programs.
- **benchmark_ext** : Benchmark Extracts with mnemonics, micro-codes built with same make flags as remaining n3350 experiments.
- **bios** : C experiment files and supporting code, built using match&patch method.
- **uasm-lib** : Mnemonics, hexcodes and *.u assembly functions used in efi experiments, invoked by bios/\*.efi
- **ghidra_glm_extension** : ghidra decompiler executable and post-processing scripts for GLM uCode extenion. This version can decompile undocumented N3350 instructions and ucodes.
- **glm_unlock** : Steps and screenshots for Goldmont+ Red Unlock. This setup is needed for dynamic tracing over JTAG/DCI.

#### HowTo
1. **Linux-Bringup on BMAX_B1_Plus**
- To bring-up linux, eMMC has to disabled on BMAX else BIOS will always default to Windows-Boot.
- On startup press the ESC or F11 key to enter the BIOS Setup, go to the “Chipset” tab. Make the following changes
```sh
HD-Audio Support → Disable
SCC eMMC Support → Disable
WIFI Support → Disable
Bluetooth Support → Disable
Boot_ → _Secure_Boot_ → _Disable
```

2. **Setting up GCC-UEFI**
**_UEFI: Hello World!_ **
- There are two possible methods used for creating UEFI executable (gnu-efi or Intel's Tianocore edk2)
- I am listing the tutorials [references] to setup a "Hello World" program before proceeding to more complex u-code patches. 
- In these experiments we use gnu-efi packages (over TianoCore EDKII) for **.efi** program development. 
- Baseline package version is used from sourceforge:**gnu-efi.3.0.14**. 
- Some differences in api/syntax has been observed, if different versions of gnu-efi are used (or tianocoreEDKII baseline used). These differences are captured in detail in tutorials[references] as UEFI framework allows firmware developers flexibility to write macros/APIs.

3. **Achieving Goldmont Unlock**
>* To update contents of MSRAM with customized micro-code patches, access to CRBUS needs to be unlocked via DCI debugger. This unlock is called Red-unlock and here we try to achive the same.
>* _Prequisites of GLM Unlock_
>1. *Intel Systems Studio*
>*Needed to integrate ME firmware (Internal ME filesystem)
>*Link here: https://www.intel.com/content/www/us/en/developer/tools/oneapi/commercial-base-iot.html#gs.70w7cl
>
>2. *Intel FIT - Flash integration Tool*
>* Reflash the MXE version for SPI flashing
>* New licenses Requires to file a support ticket and sign NDA: >https://www.intel.com/content/www/us/en/forms/support/my-intel-sign-on-support.html
>* Few archived Repositories are available (5.8.9.0): https://github.com/projectceladon/tools
>
>3. *DCI Debug Cable (Jtag over-DCI)*
>* DCI debug cable: https://www.datapro.net/products/usb-3-0-super-speed-a-a-debugging-cable.html
>
>4. *TXE firmware download*
>* (The PoC targets Intel TXE firmware version 3.0.1.1107. The SPI Flash > image for Gigabyte GB-BPCE-3350C version F5 contains the necessary firmware version)
>* Vendor specific
>* Gigabyte link: https://download.gigabyte.com/FileList/BIOS/brix_bios_gb-bpce-3350c_f5.zip
>* For other vedors (BMAX) - Isolate TXE section using FIT, update the patch.

4. **_Steps to Create Bootable EFI on USB stick_**
```sh
$lsblk -p // locate usb device to be used
$sudo umount /dev/sdb
$sudo mkfs.vfat -F 32 /dev/sdb
mkfs.fat 4.1 (2017-01-24)
$sudo mount /dev/sdb /media/usb
$sudo mkdir -p efi/boot/
$cd efi/boot/
$sudo wget -q -O BOOTX64.efi https://github.com/tianocore/edk2/raw/edk2-stable201903/ShellBinPkg/UefiShell/X64/Shell.efi  // for x86_64 machines
```

5. **_Run PAC / PACMAN Case-Studies_**
```sh
git clone https://github.com/ssrg-vt/n3350_pacman.git
cd n3350_pacman               //Update Makefile: GNU_EFI_DIR=<Gnu_EFI_home>
Make all;
ls efi_executable/cpu.efi    // CustomProcessingUnit experiments packed in this efi executable.
ls efi_executable/pac.efi    // PAC weak-sign match&patch executable
ls efi_executable/pacman.efi // PAC weak-sign and PACMAN gadget test executable
// Copy n3350_pacman/efi_execuatbles/*.efi to UEFI bootable media.
In_EFI_SHELL>cpu.efi <experiment_id> // <experiment_id=2 for PAC on x86 and 3 for PACMAN attack>
```

6. **_Static Decompilation Using Ghidra extension_**
- One of the methods used by authors of Woot'23 papers to use post-processing scripts to translate glm binaries to micro-code and then to raw micro-code file could be loaded into Ghidra decompiler IDE for static analysis.
- Ghidra could be downloaded from NSA github as an pre-built execuatbles (while using standard x86/aarch64/riscv ISA ) or could be downloaded as source code and built with custom extensions.
- Parsing through raw uCode file generated by glm_ucode_disasm; Generated ucode_glm.txt program code could be mapped in Ghidra GUI for static analysis
* **To Run Ghidra with glm_ucode extension**
```sh
git clone https://github.com/ssrg-vt/n3350_pacman.git
// To generate RAW uCode file for decompilation
cd ghidra_glm_ucode_extension/uCodedisasm/glm_ucode_disasm
glm_ucode_disasm.py ../ucode/ms_arry0.txt; cat ../ucode/ucode_glm.txt
// To decompile RAW x86-64 uCode
cd ghidra_glm_ucode_extension/ghidra_10.4_glm_ucode_extension/
./ghidraRun & // Create a new project and read-in ../ucode/ucode_glm.txt raw file. Select x86-64-LE glm_ucode format.
```

7. **_Steps to build BaggyBounds Patch__**
```sh
git clone https://github.com/ssrg-vt/n3350_pacman.git
cd n3350_pacman               //Update Makefile: GNU_EFI_DIR=<Gnu_EFI_home>
Make all
ls efi_executables/baggybounds.efi  //Built efi executable will be placed here
```

**ScreenShots**
>PAC Patch:
<img src="https://github.com/ssrg-vt/n3350_pacman/blob/main/pac_patch.jpeg" style="height: 400px; width:600px;"/>
>PACMAN Test:
<img src="https://github.com/ssrg-vt/n3350_pacman/blob/main/pacman.jpeg" style="height: 400px; width:600px;"/>
>BaggyBounds
<img src="https://github.com/ssrg-vt/n3350_pacman/blob/main/baggybound.jpeg" style="height: 400px; width:600px;"/>
> BaggyBounds LmBench
<img src="https://github.com/ssrg-vt/n3350_pacman/blob/main/lmbench_bb.jpeg" style="height: 400px; width:600px;"/>
>
>
>
>
>
>
>
>
**_References:_**
- **CustomProcessingUnit** Reverse Engineering and Customization of Intel Microcode : https://pietroborrello.com/publication/woot23/woot23.pdf
- **Microcode patches** used by Pietro Borello et.al : https://github.com/pietroborrello/CustomProcessingUnit/tree/master/ucode_collection
- **Microcode disambugation, Intel's undocumented instructions** by Mark Ermolov et.al : https://github.com/chip-red-pill/uCodeDisasm.git
- **Intel Management Engine JTAG Proof of Concept** by Mark Ermolov. et.al : https://github.com/ptresearch/IntelTXE-PoC
- **CHEx86** Context-Sensitive Enforcement of Memory Safety via Microcode-Enabled Capabilities : https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=9138994
- **Intel Celeron N3350** https://www.cpu-world.com/CPUs/Celeron/Intel-Mobile%20Celeron%20N3350.html
- **BMAX B1 Plus** https://www.bmaxit.com/2020/minipc_0808/34.html
- **ARM Pointer Authentication Code**:: https://community.arm.com/arm-community-blogs/b/architectures-and-processors-blog/posts/armv8-1-m-pointer-authentication-and-branch-target-identification-extension
- **ARM Project Morrello**:: https://developer.arm.com/Architectures/Morello
- **GNU-EFI Tutorials::** https://www.rodsbooks.com/efi-programming/prepare.html


