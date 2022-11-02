mkdir ~/Downloads/vim_files
wget https://github.com/michalmonday/files/raw/master/vim.tar.gz -P ~/Downloads
tar -xvf ~/Downloads/vim.tar.gz -C ~/Downloads/vim_files
cp ~/Downloads/vim_files/.vim/* ~/.vim -r
cp ~/Downloads/vim_files/.vimrc ~
rm ~/Downloads/vim_files -r



