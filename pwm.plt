set terminal png
set output './pwmkurve.png'
set xrange [0:255]
set yrange [0:12]
set title 'PWM-Kurve'
set ylabel 'Spannung [V]'
set xlabel 'PWM-Ratio (value/255)'
set xtics 16
set ytics 0.5
set grid
plot 'Pwmkurve' with lines, \
     'Pwmkurve_ref' with lines


