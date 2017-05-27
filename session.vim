let SessionLoad = 1
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd ~/Documents/Projects/Vedon
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +66 src/app.re
argglobal
silent! argdel *
edit src/app.re
set splitbelow splitright
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
2wincmd h
wincmd w
wincmd w
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd t
set winminheight=1 winminwidth=1 winheight=1 winwidth=1
exe 'vert 1resize ' . ((&columns * 77 + 116) / 233)
exe 'vert 2resize ' . ((&columns * 77 + 116) / 233)
exe '3resize ' . ((&lines * 30 + 31) / 63)
exe 'vert 3resize ' . ((&columns * 77 + 116) / 233)
exe '4resize ' . ((&lines * 30 + 31) / 63)
exe 'vert 4resize ' . ((&columns * 77 + 116) / 233)
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
3,6fold
8,11fold
13,20fold
22,25fold
29,32fold
27,33fold
41,50fold
123,126fold
128,137fold
74,138fold
140,144fold
148,151fold
152,155fold
147,160fold
146,161fold
166,172fold
165,173fold
175,180fold
182,185fold
188,192fold
187,195fold
200,203fold
214,216fold
211,217fold
208,218fold
242,244fold
241,245fold
219,246fold
205,247fold
197,248fold
257,261fold
250,266fold
269,273fold
74
normal! zo
140
normal! zc
146
normal! zo
147
normal! zo
197
normal! zo
205
normal! zo
219
normal! zo
241
normal! zo
250
normal! zo
let s:l = 200 - ((5 * winheight(0) + 30) / 61)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
200
normal! 041|
wincmd w
argglobal
edit src/app.re
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
3,6fold
8,11fold
13,20fold
22,25fold
29,32fold
27,33fold
41,50fold
123,126fold
128,137fold
74,138fold
140,144fold
148,151fold
152,155fold
147,160fold
146,161fold
166,172fold
165,173fold
175,180fold
182,185fold
188,192fold
187,195fold
208,218fold
219,246fold
205,247fold
197,248fold
257,261fold
250,266fold
74
normal! zo
140
normal! zc
146
normal! zo
147
normal! zo
165
normal! zo
197
normal! zo
205
normal! zo
let s:l = 103 - ((28 * winheight(0) + 30) / 61)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
103
normal! 050|
wincmd w
argglobal
edit src/app.re
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
3,6fold
8,11fold
13,20fold
22,25fold
29,32fold
27,33fold
41,50fold
123,126fold
128,137fold
74,138fold
140,144fold
148,151fold
152,155fold
147,160fold
146,161fold
166,172fold
165,173fold
175,180fold
182,185fold
188,192fold
187,195fold
208,218fold
219,246fold
205,247fold
197,248fold
257,261fold
250,266fold
74
normal! zo
74
normal! zc
140
normal! zc
146
normal! zo
147
normal! zo
187
normal! zo
197
normal! zo
205
normal! zo
let s:l = 61 - ((9 * winheight(0) + 15) / 30)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
61
normal! 017|
wincmd w
argglobal
edit src/app.re
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
3,6fold
8,11fold
13,20fold
22,25fold
29,32fold
27,33fold
41,50fold
123,126fold
128,137fold
74,138fold
140,144fold
148,151fold
152,155fold
147,160fold
146,161fold
166,172fold
165,173fold
175,180fold
182,185fold
188,192fold
187,195fold
208,218fold
219,246fold
205,247fold
197,248fold
257,261fold
250,266fold
74
normal! zo
74
normal! zc
140
normal! zc
146
normal! zo
147
normal! zo
187
normal! zo
197
normal! zo
205
normal! zo
let s:l = 185 - ((3 * winheight(0) + 15) / 30)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
185
normal! 054|
wincmd w
4wincmd w
exe 'vert 1resize ' . ((&columns * 77 + 116) / 233)
exe 'vert 2resize ' . ((&columns * 77 + 116) / 233)
exe '3resize ' . ((&lines * 30 + 31) / 63)
exe 'vert 3resize ' . ((&columns * 77 + 116) / 233)
exe '4resize ' . ((&lines * 30 + 31) / 63)
exe 'vert 4resize ' . ((&columns * 77 + 116) / 233)
tabnext 1
if exists('s:wipebuf') && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 winminheight=1 winminwidth=1 shortmess=filnxtToOIc
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
