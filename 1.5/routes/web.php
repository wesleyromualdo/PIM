<?php

Route::group(['middleware' => 'web'], function ($api) {
    Route::get('/home/', 'HomeController@index');
    Route::get('/home/logout', 'HomeController@logout');
});

Route::get('/', function () {
    return view('welcome');
});

