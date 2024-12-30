!
! SYS_SIZE is the number of clicks (16 bytes) to be loaded.
! 0x3000 is 0x30000 bytes = 196kB, more than enough for current
! versions of linux
!
SYSSIZE = 0x3000
!
!	bootsect.s		(C) 1991 Linus Torvalds
!
! bootsect.s is loaded at 0x7c00 by the bios-startup routines, and moves
! iself out of the way to address 0x90000, and jumps there.
!
! It then loads 'setup' directly after itself (0x90200), and the system
! at 0x10000, using BIOS interrupts. 
!
! NOTE! currently system is at most 8*65536 bytes long. This should be no
! problem, even in the future. I want to keep it simple. This 512 kB
! kernel size should be enough, especially as this doesn't contain the
! buffer cache as in minix
!
! The loader has been made as simple as possible, and continuos
! read errors will result in a unbreakable loop. Reboot by hand. It
! loads pretty fast by getting whole sectors at a time whenever possible.

.globl begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:
.data
begdata:
.bss
begbss:
.text

BOOTSEG  = 0x07c0			! original address of boot-sector
INITSEG  = 0x9000			! we move boot here - out of the way
SETUPSEG = 0x9020			! setup starts here
SYSSEG   = 0x1000			! system loaded at 0x10000 (65536).
ENDSEG   = SYSSEG + SYSSIZE	! where to stop loading

! ROOT_DEV:	0x000 - same type of floppy as boot.
!		0x301 - first partition on first drive etc
ROOT_DEV = 0x306

entry _start
_start:
	mov	ax,#BOOTSEG
	mov	ds,ax
	mov	ax,#SETUPSEG
	mov	es,ax
	call	print_setup
	call	print_hwinfo
	jmp	do_loop

! infinite loop
do_loop:
	jmp	do_loop

print_setup:
	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10
	
	mov	cx,#25
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#setup_msg
	mov	ax,#0x1301		! write string, move cursor
	int	0x10
	ret

print_hwinfo:
! save to INITSEG
	mov	ax,#INITSEG		! this is done in bootsect already, but...
	mov	ds,ax

! get cursor pos
	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10			! save it in known place, con_init fetches
	mov	[0],dx			! it from 0x90000.

! get memory size
	mov	ah,#0x88
	int	0x15
	mov	[2],ax

! get video-card info

	mov	ah,#0x0f
	int	0x10
	mov	[4],bx		! bh = display page
	mov	[6],ax		! al = video mode, ah = window width

! check for EGA/VGA and some config parameters

	mov	ah,#0x12
	mov	bl,#0x10
	int	0x10
	mov	[8],ax
	mov	[10],bx
	mov	[12],cx

! get disk info
	mov	ax,#0x0000
	mov	ds,ax
	lds	si,[4*0x41]
	mov	ax,#INITSEG
	mov	es,ax
	mov	di,#0x0080
	mov	cx,#0x10
	rep
	movsb

! start print
	mov ax,cs
	mov es,ax
	mov	ax,#INITSEG		! this is done in bootsect already, but...
	mov	ds,ax

! print cursor pos
	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10

	mov	cx,#12
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#cursor_msg
	mov	ax,#0x1301		! write string, move cursor
	int	0x10

	mov dx,[0]
	call	print_hex
	call	print_nl

! print memory size
	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10

	mov	cx,#13
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#memory_msg
	mov	ax,#0x1301		! write string, move cursor
	int	0x10

	mov dx,[2]
	call	print_hex

	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10

	mov	cx,#2
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#kb_msg
	mov	ax,#0x1301		! write string, move cursor
	int	0x10

	call	print_nl

! print display page
	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10

	mov	cx,#14
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#display_page_msg
	mov	ax,#0x1301		! write string, move cursor
	int	0x10

	mov dx,[4]
	call	print_hex
	call	print_nl

! print video mode
	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10

	mov	cx,#12
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#video_mode_msg
	mov	ax,#0x1301		! write string, move cursor
	int	0x10

	mov dx,[6]
	call	print_half
	call	print_nl

! print window width
	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10

	mov	cx,#14
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#window_width_msg
	mov	ax,#0x1301		! write string, move cursor
	int	0x10

	mov dx,[7]
	call	print_half
	call	print_nl

! print cylinders
	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10

	mov	cx,#6
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#cylinder_msg
	mov	ax,#0x1301		! write string, move cursor
	int	0x10

	mov dx,[0x0080]		! 0x00 -> cylinders
	call	print_hex
	call	print_nl

! print heads
	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10

	mov	cx,#7
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#head_msg
	mov	ax,#0x1301		! write string, move cursor
	int	0x10

	mov dx,[0x0082]		! 0x02 -> heads
	call	print_hex
	call	print_nl

! print sectors
	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10

	mov	cx,#9
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#sector_msg
	mov	ax,#0x1301		! write string, move cursor
	int	0x10

	mov dx,[0x008E]		! 0x0E -> sectors
	call	print_hex
	call	print_nl

	ret

print_hex:
	mov	cx,#4
	!mov	dx,(bp)		! data already loaded into dx
print_digit:
	rol	dx,#4			! high 4bit -> low 4bit
	mov	ax,#0xe0f
	and	al,dl			! dl's low 4 bit
	add	al,#0x30		! +0x30
	cmp	al,#0x3a
	jl	outp			! 0~9
	add	al,#0x07		! a~f add 7
outp: 
	int	0x10
	loop	print_digit
	ret
print_half:
	mov	cx,#2
print_half_digit:
	rol	dx,#4			! high 4bit -> low 4bit
	mov	ax,#0xe0f
	and	al,dl			! dl's low 4 bit
	add	al,#0x30		! +0x30
	cmp	al,#0x3a
	jl	outp_half		! 0~9
	add	al,#0x07		! a~f add 7
outp_half: 
	int	0x10
	loop	print_half_digit
	ret
print_nl:
	mov	ax,#0xe0d		! CR
	int	0x10
	mov	al,#0xa			! LF
	int	0x10
	ret

! string of setup message
setup_msg:
	.byte	13,10
	.ascii	"Now we are in SETUP"
	.byte	13,10,13,10

cursor_msg:
	.ascii	"Cursor POS: "

memory_msg:
	.ascii	"Memory SIZE: "

display_page_msg:
	.ascii	"Display Page: "

video_mode_msg:
	.ascii	"Video Mode: "

window_width_msg:
	.ascii	"Window Width: "

cylinder_msg:
	.ascii	"Cyls: "

head_msg:
	.ascii	"Heads: "

sector_msg:
	.ascii	"Sectors: "

kb_msg:
	.ascii	"KB"

.text
endtext:
.data
enddata:
.bss
endbss:
