#!/bin/sh
# This script sets up a (very) basic set of firewall rules

# First set all the rules to drop (nothing gets through)
iptables -P INPUT DROP
iptables -P OUTPUT DROP
iptables -P FORWARD DROP

# Flush out old rules (start with empty rules)
iptables -F


# let internal machines access the external DNS in both directions
# by using this machine as a DNS proxy
## (ie we trust this external machine but only on port 53)
iptables -A INPUT -i eth0 -p udp --dport 53 -j ACCEPT
iptables -A OUTPUT -o eth0 -p udp --sport 53 -j ACCEPT
iptables -A OUTPUT -o eth1 -p udp --dport 53 -j ACCEPT
iptables -A INPUT -i eth1 -p udp --sport 53 -j ACCEPT

# allow client to connect to gateway and server on port 22 (so that you can use wireshark)
iptables -A INPUT -i eth0 -p tcp --dport 22 -j ACCEPT
iptables -A OUTPUT -o eth0 -p tcp --sport 22 -j ACCEPT
iptables -A FORWARD -i eth0 -p tcp --dport 22 -j ACCEPT
iptables -A FORWARD -o eth0 -p tcp ! --syn --sport 22  -j ACCEPT

# below here this is where you put your configuration

# TCP traffic
iptables -A FORWARD -i eth0 -o eth1 -p tcp --dport 80 -j ACCEPT
# iptables -A FORWARD -i eth1 -o eth0 -p tcp --sport 80 -j ACCEPT
iptables -A FORWARD -i eth1 -o eth0 -p tcp ! --syn --sport 80 -j ACCEPT

# block UDP
iptables -A INPUT -p udp -j DROP
iptables -A OUTPUT -p udp -j DROP


# block outbound Telnet, pop3, pop2, imap, imap3
iptables -A OUTPUT -p tcp --sport 23 -j DROP
iptables -A OUTPUT -p tcp --sport 110 -j DROP
iptables -A OUTPUT -p tcp --sport 109 -j DROP
iptables -A OUTPUT -p tcp --sport 993 -j DROP
iptables -A OUTPUT -p tcp --sport 220 -j DROP

iptables -A OUTPUT -p tcp --dport 23 -j DROP
iptables -A OUTPUT -p tcp --dport 110 -j DROP
iptables -A OUTPUT -p tcp --dport 109 -j DROP
iptables -A OUTPUT -p tcp --dport 993 -j DROP
iptables -A OUTPUT -p tcp --dport 220 -j DROP

# block all smtp
# iptables -A INPUT -p tcp --sport 25 -j DROP
# iptables -A INPUT -p tcp --sport 465 -j DROP
# iptables -A INPUT -p tcp --sport 587 -j DROP
# iptables -A INPUT -p tcp --dport 25 -j DROP
# iptables -A INPUT -p tcp --dport 465 -j DROP
# iptables -A INPUT -p tcp --dport 587 -j DROP
# iptables -A OUTPUT -p tcp --sport 25 -j DROP
# iptables -A OUTPUT -p tcp --sport 465 -j DROP
# iptables -A OUTPUT -p tcp --sport 587 -j DROP
# iptables -A OUTPUT -p tcp --dport 25 -j DROP
# iptables -A OUTPUT -p tcp --dport 465 -j DROP
# iptables -A OUTPUT -p tcp --dport 587 -j DROP

# block all ftp
iptables -A INPUT -p tcp --sport 21 -j DROP
iptables -A INPUT -p tcp --dport 21 -j DROP
iptables -A OUTPUT -p tcp --sport 21 -j DROP
iptables -A OUTPUT -p tcp --dport 21 -j DROP

# block all netbios protocols

# block all TCP connections from External to Internal except SSH traffic from External to Internal
iptables -A INPUT -p tcp -j DROP

# accept any other outgoing tcp traffic
iptables -A OUTPUT -p tcp -j ACCEPT











