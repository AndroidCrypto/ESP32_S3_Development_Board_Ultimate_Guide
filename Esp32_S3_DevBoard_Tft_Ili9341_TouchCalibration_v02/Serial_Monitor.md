ESP32-S3 TFT ILI9341 Touch Calibration V02
=== Collecting data for the top left corner ===
Nr sample:  0 | X =  437 | Y =  362 | Z =  409
Nr sample:  1 | X =  421 | Y =  363 | Z =  566
Nr sample:  2 | X =  414 | Y =  357 | Z =  567
Nr sample:  3 | X =  407 | Y =  368 | Z =  570
Nr sample:  4 | X =  413 | Y =  360 | Z =  579
Nr sample:  5 | X =  411 | Y =  365 | Z =  583
Nr sample:  6 | X =  418 | Y =  369 | Z =  577
Nr sample:  7 | X =  423 | Y =  361 | Z =  580
Nr sample:  8 | X =  423 | Y =  364 | Z =  589
Nr sample:  9 | X =  420 | Y =  374 | Z =  583
Nr sample: 10 | X =  419 | Y =  359 | Z =  596
Nr sample: 11 | X =  419 | Y =  359 | Z =  580
Nr sample: 12 | X =  419 | Y =  354 | Z =  546
Nr sample: 13 | X =  409 | Y =  365 | Z =  583
Nr sample: 14 | X =  397 | Y =  361 | Z =  574
Nr sample: 15 | X =  391 | Y =  372 | Z =  578
Nr sample: 16 | X =  385 | Y =  371 | Z =  553
Nr sample: 17 | X =  379 | Y =  372 | Z =  557
Nr sample: 18 | X =  378 | Y =  378 | Z =  474
Nr sample: 19 | X =  379 | Y =  383 | Z =  569
=== Collecting data for the bottom right corner ===
Nr sample:  0 | X = 3693 | Y = 3840 | Z = 1215
Nr sample:  1 | X = 3726 | Y = 3857 | Z = 2250
Nr sample:  2 | X = 3733 | Y = 3859 | Z = 2293
Nr sample:  3 | X = 3731 | Y = 3859 | Z = 2368
Nr sample:  4 | X = 3734 | Y = 3862 | Z = 2438
Nr sample:  5 | X = 3741 | Y = 3862 | Z = 2450
Nr sample:  6 | X = 3740 | Y = 3865 | Z = 2458
Nr sample:  7 | X = 3735 | Y = 3864 | Z = 2464
Nr sample:  8 | X = 3743 | Y = 3865 | Z = 2471
Nr sample:  9 | X = 3741 | Y = 3861 | Z = 2437
Nr sample: 10 | X = 3742 | Y = 3860 | Z = 2428
Nr sample: 11 | X = 3742 | Y = 3861 | Z = 2438
Nr sample: 12 | X = 3749 | Y = 3861 | Z = 2439
Nr sample: 13 | X = 3764 | Y = 3862 | Z = 2447
Nr sample: 14 | X = 3741 | Y = 3861 | Z = 2463
Nr sample: 15 | X = 3751 | Y = 3861 | Z = 2473
Nr sample: 16 | X = 3750 | Y = 3862 | Z = 2485
Nr sample: 17 | X = 3750 | Y = 3862 | Z = 2498
Nr sample: 18 | X = 3772 | Y = 3862 | Z = 2473
Nr sample: 19 | X = 3750 | Y = 3862 | Z = 2479
--== Calibration Data ==--
x0  408 x1 3741 y0  365 y1 3860
use this mapping:
x = map(p.x, 408, 3741, 1, SCREEN_WIDTH);
y = map(p.y, 365, 3860, 1, SCREEN_HEIGHT);
--== Calibration Data End ==--