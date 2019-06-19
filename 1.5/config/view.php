<?php

return [
    'cache' => false,
    'paths' => [
        realpath(base_path('resources/views')),
    ],
    'compiled' => env('VIEW_DIR', '/tmp/view'),
];
