# ttyecho
a tool to execute command to another pty

## Compile
make ttyecho

## Execute
sudo ./ttyecho -n /dev/pts/? ls

You need to be root to send the ioctl

## See
http://www.humbug.in/2010/utility-to-send-commands-or-data-to-other-terminals-ttypts/
