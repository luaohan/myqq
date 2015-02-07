if &cp | set nocp | endif
nmap  :bn
nmap  :bp
nmap d :cs find d =expand("<cword>")	
nmap i :cs find i ^=expand("<cfile>")$
nmap f :cs find f =expand("<cfile>")	
nmap e :cs find e =expand("<cword>")	
nmap t :cs find t =expand("<cword>")	
nmap c :cs find c =expand("<cword>")	
nmap g :cs find g =expand("<cword>")	
nmap s :cs find s =expand("<cword>")	
vnoremap <silent> ,,w :call EasyMotion#WB(1, 0)
onoremap <silent> ,,w :call EasyMotion#WB(0, 0)
nnoremap <silent> ,,w :call EasyMotion#WB(0, 0)
vnoremap <silent> ,,t :call EasyMotion#T(1, 0)
onoremap <silent> ,,t :call EasyMotion#T(0, 0)
nnoremap <silent> ,,t :call EasyMotion#T(0, 0)
vnoremap <silent> ,,n :call EasyMotion#Search(1, 0)
onoremap <silent> ,,n :call EasyMotion#Search(0, 0)
nnoremap <silent> ,,n :call EasyMotion#Search(0, 0)
vnoremap <silent> ,,k :call EasyMotion#JK(1, 1)
onoremap <silent> ,,k :call EasyMotion#JK(0, 1)
nnoremap <silent> ,,k :call EasyMotion#JK(0, 1)
vnoremap <silent> ,,j :call EasyMotion#JK(1, 0)
onoremap <silent> ,,j :call EasyMotion#JK(0, 0)
nnoremap <silent> ,,j :call EasyMotion#JK(0, 0)
vnoremap <silent> ,,gE :call EasyMotion#EW(1, 1)
onoremap <silent> ,,gE :call EasyMotion#EW(0, 1)
nnoremap <silent> ,,gE :call EasyMotion#EW(0, 1)
vnoremap <silent> ,,f :call EasyMotion#F(1, 0)
onoremap <silent> ,,f :call EasyMotion#F(0, 0)
nnoremap <silent> ,,f :call EasyMotion#F(0, 0)
vnoremap <silent> ,,e :call EasyMotion#E(1, 0)
onoremap <silent> ,,e :call EasyMotion#E(0, 0)
nnoremap <silent> ,,e :call EasyMotion#E(0, 0)
vnoremap <silent> ,,b :call EasyMotion#WB(1, 1)
onoremap <silent> ,,b :call EasyMotion#WB(0, 1)
nnoremap <silent> ,,b :call EasyMotion#WB(0, 1)
vnoremap <silent> ,,W :call EasyMotion#WBW(1, 0)
onoremap <silent> ,,W :call EasyMotion#WBW(0, 0)
nnoremap <silent> ,,W :call EasyMotion#WBW(0, 0)
vnoremap <silent> ,,T :call EasyMotion#T(1, 1)
onoremap <silent> ,,T :call EasyMotion#T(0, 1)
nnoremap <silent> ,,T :call EasyMotion#T(0, 1)
vnoremap <silent> ,,N :call EasyMotion#Search(1, 1)
onoremap <silent> ,,N :call EasyMotion#Search(0, 1)
nnoremap <silent> ,,N :call EasyMotion#Search(0, 1)
vnoremap <silent> ,,ge :call EasyMotion#E(1, 1)
onoremap <silent> ,,ge :call EasyMotion#E(0, 1)
nnoremap <silent> ,,ge :call EasyMotion#E(0, 1)
vnoremap <silent> ,,F :call EasyMotion#F(1, 1)
onoremap <silent> ,,F :call EasyMotion#F(0, 1)
nnoremap <silent> ,,F :call EasyMotion#F(0, 1)
vnoremap <silent> ,,E :call EasyMotion#EW(1, 0)
onoremap <silent> ,,E :call EasyMotion#EW(0, 0)
nnoremap <silent> ,,E :call EasyMotion#EW(0, 0)
vnoremap <silent> ,,B :call EasyMotion#WBW(1, 1)
onoremap <silent> ,,B :call EasyMotion#WBW(0, 1)
nnoremap <silent> ,,B :call EasyMotion#WBW(0, 1)
nmap ,ihn :IHN
nmap ,is :IHS:A
nmap ,ih :IHS
nmap ,n :NERDTreeToggle
nmap ,t :TagbarToggle
nmap ,a :A
nmap <silent> ,/ :nohls
nmap ,f9 :set foldlevel=9
nmap ,f8 :set foldlevel=8
nmap ,f7 :set foldlevel=7
nmap ,f6 :set foldlevel=6
nmap ,f5 :set foldlevel=5
nmap ,f4 :set foldlevel=4
nmap ,f3 :set foldlevel=3
nmap ,f2 :set foldlevel=2
nmap ,f1 :set foldlevel=1
nmap ,f0 :set foldlevel=0
nmap ,p :setlocal paste!
nmap ,w! :w !sudo tee % >/dev/null
nmap ,wa :wa!
nmap ,ww :w!
nmap ,wq :wqa!
nmap ,q :qa!
nnoremap ; :
vnoremap < <gv
vnoremap = =gv
vnoremap > >gv
vmap [% [%m'gv``
vmap ]% ]%m'gv``
vmap a% [%v]%
let s:cpo_save=&cpo
set cpo&vim
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
nmap <Nul><Nul>d :vert scs find d =expand("<cword>")
nmap <Nul><Nul>i :vert scs find i ^=expand("<cfile>")$	
nmap <Nul><Nul>f :vert scs find f =expand("<cfile>")	
nmap <Nul><Nul>e :vert scs find e =expand("<cword>")
nmap <Nul><Nul>t :vert scs find t =expand("<cword>")
nmap <Nul><Nul>c :vert scs find c =expand("<cword>")
nmap <Nul><Nul>g :vert scs find g =expand("<cword>")
nmap <Nul><Nul>s :vert scs find s =expand("<cword>")
nmap <Nul>d :scs find d =expand("<cword>")	
nmap <Nul>i :scs find i ^=expand("<cfile>")$	
nmap <Nul>f :scs find f =expand("<cfile>")	
nmap <Nul>e :scs find e =expand("<cword>")	
nmap <Nul>t :scs find t =expand("<cword>")	
nmap <Nul>c :scs find c =expand("<cword>")	
nmap <Nul>g :scs find g =expand("<cword>")	
nmap <Nul>s :scs find s =expand("<cword>")	
imap ,ihn :IHN
imap ,is :IHS:A
imap ,ih :IHS
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set autoread
set background=dark
set backspace=indent,eol,start
set backup
set backupdir=~/.tmp,/tmp
set cscopeprg=/usr/bin/cscope
set cscopequickfix=s-,c-,d-,i-,t-,e-,g-
set cscopetag
set cscopeverbose
set expandtab
set fileencodings=utf-8,ucs-bom,chinese,taiwan,latin1
set fileformats=unix,dos,mac
set gdefault
set guicursor=n-v-c:block,o:hor50,i-ci:hor15,r-cr:hor30,sm:block,a:blinkon0
set helplang=en
set hidden
set history=100
set hlsearch
set ignorecase
set incsearch
set laststatus=2
set lazyredraw
set matchpairs=(:),[:],{:},<:>
set matchtime=2
set path=.,/usr/local/include,/usr/include,/usr/include/c++/**,./include,../include
set report=0
set ruler
set runtimepath=~/.vim,~/.vim/bundle/a.vim,~/.vim/bundle/indent-motion,~/.vim/bundle/matchit.zip,~/.vim/bundle/my.vim,~/.vim/bundle/nerdtree,~/.vim/bundle/tagbar,~/.vim/bundle/vim-easymotion,~/.vim/bundle/vim-fugitive,~/.vim/bundle/vim-pathogen,~/.vim/bundle/xmledit,/usr/share/vim/vimfiles,/usr/share/vim/vim72,/usr/share/vim/vimfiles/after,~/.vim/after
set scrolljump=3
set scrolloff=7
set shiftwidth=4
set shortmess=filmnrxoOtT
set showcmd
set showmatch
set smartcase
set smartindent
set smarttab
set softtabstop=4
set statusline=[%F]%y%r%m%*%=[Line:%l/%L,Column:%c][%p%%]
set noswapfile
set tabstop=4
set termencoding=utf-8
set timeoutlen=500
set whichwrap=b,s,<,>,h,l
set wildignore=*~,*.o,*.so,*.so.*,*.a,*.d,*.pyc,*.pyo,*.class
set wildmenu
set wildmode=list,longest,full
" vim: set ft=vim :
