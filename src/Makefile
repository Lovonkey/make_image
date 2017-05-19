all:draw_frame draw_side

draw_frame: draw_frame.o
	gcc -Wall -O2 $^ -o $@

draw_side:draw_side.o
	gcc -Wall -O2 $^ -o $@

draw_frame.o: draw_frame.c
	gcc -Wall -O2 -c $^ -o $@

draw_side.o: draw_side.c
	gcc -Wall -O2 -c $^ -o $@

clean:
	rm *.o
