" Vim syntax file
"
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
syn clear
syn case ignore

syn match tse_func "([^)]*)"
hi link tse_func   Comment

syn match tse_error /^...... E .*/
hi tse_error guibg=red guifg=black

syn match tse_warn /^...... W .*/
hi tse_warn  guibg=yellow guifg=black

syn match tse_info / I /
hi tse_info  guibg=lightgreen guifg=black

syn match tse_debug / D /
hi tse_debug guibg=lightblue  guifg=black

syn match tse_trace /^...... T .*/
"hi tse_trace  guibg=white guifg=magenta
hi tse_trace  guibg=black guifg=magenta

syn match tse_zcm /ZCM /
hi tse_zcm  guibg=black guifg=yellow

syn match tse_zqm /ZQM /
hi tse_zqm  guibg=lightred guifg=black

syn match tse_newconn /new connection with.*/
hi tse_newconn  guibg=lightblue guifg=black

let b:current_syntax = 'tse'
