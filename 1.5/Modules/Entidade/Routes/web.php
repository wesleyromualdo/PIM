<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/


//Route::group(['middleware' => 'web', 'prefix' => 'seguranca', 'namespace' => 'Modules\Seguranca\Http\Controllers'], function () {
Route::prefix('seguranca')->group(function() {
    Route::get('/', 'SegurancaController@index');

    Route::any('perfil/getList', [
        'as' => 'perfil.getList',
        'uses' => 'PerfilController@getList'
    ]);

    Route::resource('perfil', 'PerfilController');

    Route::any('perfilglobal/getList', [
        'as' => 'perfilglobal.getList',
        'uses' => 'PerfilGlobalController@getList'
    ]);

    Route::resource('perfilglobal', 'PerfilGlobalController');

    Route::any('menu/getList', [
        'as' => 'menu.getList',
        'uses' => 'MenuController@getList'
    ]);
    Route::resource('menu', 'MenuController');

    Route::any('menuglobal/getList', [
        'as' => 'menuglobal.getList',
        'uses' => 'MenuGlobalController@getList'
    ]);
    Route::resource('menuglobal', 'MenuGlobalController');


    Route::any('perfilmenu/getList', [
        'as' => 'perfilmenu.getList',
        'uses' => 'PerfilMenuController@getList'
    ]);

    Route::any('perfilmenu/associarMenu', [
        'as' => 'perfilmenu.associarMenu',
        'uses' => 'PerfilMenuController@associarMenu'
    ]);

    Route::resource('perfilmenu', 'PerfilMenuController');

    Route::any('perfilmenuglobal/getList', [
        'as' => 'perfilmenuglobal.getList',
        'uses' => 'PerfilMenuGlobalController@getList'
    ]);

    Route::any('perfilmenuglobal/associarMenu', [
        'as' => 'perfilmenuglobal.associarMenu',
        'uses' => 'PerfilMenuGlobalController@associarMenu'
    ]);

    Route::any('perfilRota/associarRota', [
        'as' => 'perfilRota.associarRota',
        'uses' => 'PerfilRotaController@associarRota'
    ]);

    Route::any('perfilmenuglobal/ListarPerfilBySistema', [
        'as' => 'perfilmenuglobal.ListarPerfilBySistema',
        'uses' => 'PerfilMenuGlobalController@ListarPerfilBySistema'
    ]);

    Route::resource('perfilmenuglobal', 'PerfilMenuGlobalController');

    Route::any('usuario/consultarUsuarios', [
        'as' => 'usuario.consultarUsuarios',
        'uses' => 'UsuarioController@consultarUsuarios'
    ]);


    Route::any('usuario/getList', [
        'as' => 'usuario.getList',
        'uses' => 'UsuarioController@getList'
    ]);

    Route::any('usuario/GeraTabelaSimularUsuario', [
        'as' => 'usuario.GeraTabelaSimularUsuario',
        'uses' => 'UsuarioController@GeraTabelaSimularUsuario'
    ]);

    Route::any('usuario/SimulaUsuario', [
        'as' => 'usuario.SimulaUsuario',
        'uses' => 'UsuarioController@SimulaUsuario'
    ]);
    Route::any('usuario/VoltaUsuario', [
        'as' => 'usuario.VoltaUsuario',
        'uses' => 'UsuarioController@VoltaUsuario'
    ]);
    Route::resource('usuario', 'UsuarioController');

    Route::any('usuarioglobal/consultarUsuarios', [
        'as' => 'usuarioglobal.consultarUsuarios',
        'uses' => 'UsuarioGlobalController@consultarUsuarios'
    ]);

    Route::any('usuarioglobal/getList', [
        'as' => 'usuarioglobal.getList',
        'uses' => 'UsuarioGlobalController@getList'
    ]);

    Route::any('usuarioglobal/GeraTabelaSimularUsuario', [
        'as' => 'usuarioglobal.GeraTabelaSimularUsuario',
        'uses' => 'UsuarioGlobalController@GeraTabelaSimularUsuario'
    ]);

    Route::any('usuarioglobal/SimulaUsuario', [
        'as' => 'usuarioglobal.SimulaUsuario',
        'uses' => 'UsuarioGlobalController@SimulaUsuario'
    ]);
    Route::any('usuarioglobal/VoltaUsuario', [
        'as' => 'usuarioglobal.VoltaUsuario',
        'uses' => 'UsuarioGlobalController@VoltaUsuario'
    ]);
    Route::resource('usuarioglobal', 'UsuarioGlobalController');

    Route::any('usuarioresponsabilidade/getList', [
        'as' => 'usuarioresponsabilidade.getList',
        'uses' => 'UsuarioResponsabilidadeController@getList'
    ]);

    Route::any('usuarioresponsabilidade/associarResponsabilidade', [
        'as' => 'usuarioresponsabilidade.associarResponsabilidade',
        'uses' => 'UsuarioResponsabilidadeController@associarResponsabilidade'
    ]);

//    Route::any('usuarioresponsabilidade/usucpf/{usucpf}', [
//                'as' => 'usuarioresponsabilidade.editResponsabilidade',
//               'uses'=>'UsuarioResponsabilidadeController@index'
//    ]);

    Route::any('usuarioresponsabilidade/geraTabelaResponsabilidadesUsuario', [
        'as' => 'usuarioresponsabilidade.geraTabelaResponsabilidadesUsuario',
        'uses' => 'UsuarioResponsabilidadeController@geraTabelaResponsabilidadesUsuario'
    ]);

    Route::any('usuarioresponsabilidade/renderView', [
        'as' => 'usuarioresponsabilidade.renderView',
        'uses' => 'UsuarioResponsabilidadeController@renderView'
    ]);

    Route::resource('usuarioresponsabilidade', 'UsuarioResponsabilidadeController');


    Route::any('usuariosistema/getList', [
        'as' => 'usuariosistema.getList',
        'uses' => 'UsuarioSistemaController@getList'
    ]);

    Route::any('usuariosistema/pegaPerfisSistemabyUsuario', [
        'as' => 'usuariosistema.pegaPerfisSistemabyUsuario',
        'uses' => 'UsuarioSistemaController@pegaPerfisSistemabyUsuario'
    ]);

    Route::any('usuariosistema/getHistorico', [
        'as' => 'usuariosistema.getHistorico',
        'uses' => 'UsuarioSistemaController@getHistorico'
    ]);

    Route::any('usuariosistema/associarResponsabilidade', [
        'as' => 'usuariosistema.associarResponsabilidade',
        'uses' => 'UsuarioSistemaController@associarResponsabilidade'
    ]);

//    Route::any('usuariosistema/usucpf/{usucpf}', [
//                'as' => 'usuariosistema.editResponsabilidade',
//               'uses'=>'UsuarioSistemaController@index'
//    ]);

    Route::any('usuariosistema/geraTabelaResponsabilidadesUsuario', [
        'as' => 'usuariosistema.geraTabelaResponsabilidadesUsuario',
        'uses' => 'UsuarioSistemaController@geraTabelaResponsabilidadesUsuario'
    ]);


    Route::resource('usuariosistema', 'UsuarioSistemaController');


    Route::get('logErro/getList', [
        'as' => 'logErro.getList',
        'uses' => 'ErroController@getList'
    ]);

    Route::resource('logErro', 'ErroController');

    Route::post('exportarsistema/exportarSistema', [
        'as' => 'exportarsistema.exportarSistema',
        'uses' => 'ExportarSistemaController@exportarSistema'
    ]);

    Route::resource('exportarsistema', 'ExportarSistemaController');


    Route::resource('territorio', 'TerritorioController');
    Route::resource('entidade', 'EntidadeController');

    Route::get('sistema/getList', [
        'as' => 'sistema.getList',
        'uses' => 'SistemaController@getList'
    ]);

//    Route::post('sistema/exportarSistema', [
//        'as'=>'sistema.exportarSistema',
//        'uses'=>'SistemaController@Sistema'
//    ]);

    Route::resource('sistema', 'SistemaController');


    Route::any('perfilrota/getList', [
        'as' => 'perfilrota.getList',
        'uses' => 'PerfilRotaController@getList'
    ]);

    Route::any('perfilrota/getPerfil', [
        'as' => 'perfilrota.getPerfil',
        'uses' => 'PerfilRotaController@getPerfil'
    ]);

    Route::any('perfilrota/associarRota', [
        'as' => 'perfilrota.associarRota',
        'uses' => 'PerfilRotaController@associarRota'
    ]);

    Route::resource('perfilrota', 'PerfilRotaController');

    Route::any('rota/getList', [
        'as' => 'rota.getList',
        'uses' => 'RotaController@getList'
    ]);

    Route::any('rota/getListRotasBanco', [
        'as' => 'rota.getListRotasBanco',
        'uses' => 'RotaController@getListRotasBanco'
    ]);

    Route::any('rota/atualizaStatusRotaBanco', [
        'as' => 'rota.atualizaStatusRotaBanco',
        'uses' => 'RotaController@atualizaStatusRotaBanco'
    ]);

    Route::any('rota/atualizarota', [
        'as' => 'rota.atualizarota',
        'uses' => 'RotaController@atualizaRota'
    ]);

    Route::resource('rota', 'RotaController');




    Route::get('/', 'SegurancaController@index');
});
