# Link-State (LS) routing simulation

set ns [new Simulator]
$ns rtproto LS

# Trace files
set nf [open ls1.tr w]
$ns trace-all $nf

set nr [open ls2.nam w]
$ns namtrace-all $nr

proc finish {} {
    global ns nf nr

    $ns flush-trace
    close $nf
    close $nr
    exec nam ls2.nam
    exit 0
}

# Topology: 4 nodes in a ring
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]

$ns duplex-link $n0 $n1 1Mb 10ms DropTail
$ns duplex-link $n1 $n2 1Mb 10ms DropTail
$ns duplex-link $n2 $n3 1Mb 10ms DropTail
$ns duplex-link $n3 $n0 1Mb 10ms DropTail

# Flow 0: n0 -> n3
set udp0 [new Agent/UDP]
$ns attach-agent $n0 $udp0

set cbr0 [new Application/Traffic/CBR]
$cbr0 attach-agent $udp0
$cbr0 set packetSize_ 500
$cbr0 set interval_ 0.005

set null0 [new Agent/Null]
$ns attach-agent $n3 $null0
$ns connect $udp0 $null0

# Flow 1: n1 -> n3
set udp1 [new Agent/UDP]
$ns attach-agent $n1 $udp1

set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp1
$cbr1 set packetSize_ 500
$cbr1 set interval_ 0.005

set null1 [new Agent/Null]
$ns attach-agent $n3 $null1
$ns connect $udp1 $null1

# Schedule traffic and simulation end
$ns at 0.1  "$cbr1 start"
$ns at 0.2  "$cbr0 start"
$ns at 45.0 "$cbr1 stop"
$ns at 45.1 "$cbr0 stop"
$ns at 50.0 "finish"

$ns run

# Run: ns lsr.tcl
# Trace output: ls1.tr  |  NAM file: ls2.nam (opens automatically at end)
# Analyze performance: awk -f lsr.awk ls1.tr
