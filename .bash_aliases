# Cleaning the Package Cache
alias pcc='sudo pacman -Scc'

# Search packages in Arch reps & AUR using fzf

alias ps='pacman -Ssq | fzf'
alias ys='yay -Slaq | fzf'

# Pacman mirrorlist

alias pm='sudo vim /etc/pacman.d/mirrorlist'

# Pacman config

alias pc='sudo vim /etc/pacman.conf'

# Pacman logs

alias pl='vim /var/log/pacman.log'

# Suckless

alias sd='vim ~/.config/suckless/dwm/config.h'

# Ranger

alias r='vim ~/.config/ranger/rc.conf'

# Bash aliases

alias a='vim ~/.bash_aliases'
