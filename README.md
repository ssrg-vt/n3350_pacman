## n3350_pacman

![PACMAN](https://github.com/ssrg-vt/n3350_pacman/blob/main/pacman.png)

**Introduction**

This is the collection of documentation, results and uCode Patches to replicate Pointer Authentication Code Case study on Intel N3350 / Goldmont Plus Architecture
>* This work builds upon different case studies presnted in Woot'23 publication and Blackhat'22 by Pietro Borrello et.al
>* uCode patching and tracing mechanism is primarily inspired by authors published work found at: https://github.com/pietroborrello/CustomProcessingUnit

Intel Atom N3350 (Glodmont) is used architecture of choice to test implementation of Pointer Authentication Codes(PAC) on x86_64 
BMAX_B1 Plus is used as testbed to demonstrate.

**HowTo**
1. **Linux-Bringup on BMAX_B1_Plus**
>* To bring-up linux, eMMC has to disabled on BMAX else BIOS will always default to Windows-Boot.
>* On startup (quickly) press the ESC or DEL key (many times) to enter the BIOS Setup, go to the “Chipset” tab. Make the following changes
>* _HD-Audio Support → Disable_
>* _SCC eMMC Support → Disable_
>* _WIFI Support → Disable_
>* _Bluetooth Support → Disable_

2. **Goldmont Unlock**
>* To update contents of MSRAM with customized micro-code patches, access to CRBUS needs to be unlocked via DCI debugger. This unlock is called Red-unlock and here we try to achive the same.
>* _Prequisites of GLM Unlock_
>1. **Intel Systems Studio**
>*Needed to integrate ME firmware (Internal ME filesystem)
>*Link here: https://www.intel.com/content/www/us/en/developer/tools/oneapi/commercial-base-iot.html#gs.70w7cl
>
>2. **Intel FIT - Flash integration Tool**
>* Reflash the MXE version for SPI flashing
>* New licenses Requires to file a support ticket and sign NDA: >https://www.intel.com/content/www/us/en/forms/support/my-intel-sign-on-support.html
>* Few archived Repositories are available (5.8.9.0): https://github.com/projectceladon/tools
>
>3. **DCI Debug Cable (Jtag over-DCI)**
>* a) Hack USB3-to-USB3 Cables (isolate D+/- Vcc)
>* b) Or buy DCI debug cable: https://www.datapro.net/products/usb-3-0-super-speed-a-a-debugging-cable.html
>
>4. **TXE firmware download**
>* (The PoC targets Intel TXE firmware version 3.0.1.1107. The SPI Flash > image for Gigabyte GB-BPCE-3350C version F5 contains the necessary firmware version)
>* Vendor specific
>* Gigabyte link: https://download.gigabyte.com/FileList/BIOS/brix_bios_gb-bpce-3350c_f5.zip
>* For other vedors (BMAX) - Isolate TXE section using FIT, update the patch.
>   
5. **Setting up GCC-UEFI**
6. **Match & Patch**

**_UEFI: Hello World!_**
>
>
>
>
>
**ScreenShots of Results**
>
>
>
>
>
>
**_References:_**
>* **CustomProcessingUnit** Reverse Engineering and Customization of Intel Microcode : https://pietroborrello.com/publication/woot23/woot23.pdf
>* **Microcode patches** used by Pietro Borello et.al : https://github.com/pietroborrello/CustomProcessingUnit/tree/master/ucode_collection
>* **Intel Management Engine JTAG Proof of Concept** by Mark Ermolov. et.al : https://github.com/ptresearch/IntelTXE-PoC
>* **CHEx86** Context-Sensitive Enforcement of Memory Safety via Microcode-Enabled Capabilities : https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=9138994
>* **Intel Celeron N3350** https://www.cpu-world.com/CPUs/Celeron/Intel-Mobile%20Celeron%20N3350.html
>* **BMAX B1 Plus** https://www.bmaxit.com/2020/minipc_0808/34.html
>* **ARM Pointer Authentication Code**::
>* **ARM Project Morrello**:: 
