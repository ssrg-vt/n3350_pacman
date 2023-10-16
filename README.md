## n3350_pacman

![PACMAN](https://github.com/ssrg-vt/n3350_pacman/blob/main/pacman.png)

**Introduction**

This is the collection of documentation, results and uCode Patches to replicate Pointer Authentication Code Case study on Intel N3350 / Goldmont Plus Architecture
>* This work builds upon different case studies presnted in Woot'23 publication and Blackhat'22 by Pietro Borrello et.al
>* uCode patching and tracing mechanism is primarily inspired by authors published work found at: https://github.com/pietroborrello/CustomProcessingUnit

Intel Atom N3350 (Glodmont) is used architecture of choice to test implementation of Pointer Authentication Codes(PAC) on x86_64 
BMAX_B1 Plus is used as testbed to demonstrate.

**HowTo**
1. Linux-Bringup on BMAX_B1_Plus
2. Setup GCC-UEFI
3. Glodmont Unlock
4. Match & Patch
*_UEFI: Hello World!_
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
