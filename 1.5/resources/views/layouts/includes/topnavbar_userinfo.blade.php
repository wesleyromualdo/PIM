<div class="modal fade" id="modal-user" tabindex="0" role="dialog" aria-labelledby="myModalUser" aria-hidden="true">
    <div class="modal-dialog">

        <div class="panel panel-primary">
            <div class="panel-heading">
                <h3 class="panel-title">Informações de usuário</h3>
            </div>

            <div class="panel-body">
                <div class="row">

                    <div class="col-md-3 col-lg-3 text-center">
                        <img class="img-circle profile-pic img_perfil_info_usuario" src="/seguranca/imagemperfil.php" alt="{{$user->usunome}}">
                        <a href="#" class="profile-picture">Editar foto</a>
                    </div>

                    <div class="col-md-9 col-lg-9 hidden-xs hidden-sm">
                        <table class="table table-bordered table-condensed table-striped">
                            <tbody>
                            <tr>
                                <td colspan="2"><strong>{{$user->usunome}}</strong></td>
                            </tr>
                            <tr>
                                <td>CPF:</td>
                                <td>{{$user->usucpf}}</td>
                            </tr>
                            <tr>
                                <td>E-mail:</td>
                                <td>{{$user->usuemail}}</td>
                            </tr>
                            <tr>
                                <td>Perfil:</td>
                                <td>
                                    {{exibirListasdePerfis($user->usucpf, $user->sisid)}}
                                </td>
                            </tr>
                            </tbody>
                        </table>
                    </div>
                    <div class="row">
                        <div class="col-md-12 col-lg-12">
                            {{exibirListasdeAvisos($user->usucpf)}}
                        </div>
                    </div>
                </div>
            </div>

            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal"><i class="fa fa-close"></i> Fechar</button>
            </div>
        </div>

    </div>
</div>

<div class="modal fade" id="modal-simular" tabindex="0" role="dialog" aria-labelledby="myModalUser" aria-hidden="true">
    <div class="modal-dialog ">

        <div class="panel panel-primary">
            <div class="panel-heading">
                <h3 class="panel-title">Simular Usuário: {{$_SESSION['sisdsc']}}</h3>
            </div>

            <div class="panel-body">
                @include('seguranca::simularusuario.index')
            </div>

            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal"><i class="fa fa-close"></i> Fechar</button>
            </div>
        </div>

    </div>
</div>

<?php if ($_SESSION['sisid'] == 266) { ?>
<div class="modal fade" id="modal-uo" tabindex="0" role="dialog" aria-labelledby="myModalUser" aria-hidden="true">
    <div class="modal-dialog">

        <div class="panel panel-primary">
            <div class="panel-heading">
                <h3 class="panel-title">Atribuir UO a um usuário</h3>
            </div>

            <?php if (!empty($_SESSION['unidadesPerfil'])) { ?>
            <div class="panel-body">
                <div class="row">
                    <div class="col-md-12 col-lg-12 hidden-xs hidden-sm">
                        {!! Form::open(array('route' => 'recorcv2.atribuiruousuario','method'=>'POST', 'class' => 'form-horizontal')) !!}
                        <table class="table table-bordered table-condensed table-striped">
                            <tbody>
                            <tr>
                                <td>CPF:</td>
                                <td><div class="row"><div class="col-md-4 col-lg-4 hidden-xs hidden-sm">{!! Form::text('usucpf', null, array('maxlength'=>50,'data-mask' => '999.999.999-99', 'placeholder' => '','class' => 'form-control font-12 usucpffield')) !!}</div></div></td>
                            </tr>
                                <tr class="uoatual" style="display: none;">
                                    <td>UO Atual:</td>
                                    <td class="uonome">
                                    </td>
                                </tr>
                                <tr class="uoatual" style="display: none;">
                                    <td>Usuário:</td>
                                    <td class="nomeusu">
                                    </td>
                                </tr>
                    <tr>
                                <td>Atribuir nova UO:</td>
                                <td>{!! Form::select('unicod', $_SESSION['unidadesPerfil'], (!empty($uniOrcid) ? $uniOrcid : null), ['class' => 'form-control font-12', 'placeholder' => 'Selecione...', 'id' => 'unicod'])!!}</td>
                            </tr>
                            <tr>
                                <td colspan="2"><button type="submit" class="btn btn-success" id="btn-atribuir-uo" style="float: right; font-size: 12px">Atribuir</button></td>
                            </tr>
                            </tbody>
                        </table>
                        {!! Form::close() !!}
                    </div>
                    <div class="row">
                        <div class="col-md-12 col-lg-12">
                            {{exibirListasdeAvisos($user->usucpf)}}
                        </div>
                    </div>
                </div>
            </div>
            <?php } ?>

            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal"><i class="fa fa-close"></i> Fechar</button>
            </div>
        </div>

    </div>
</div>

@section('myjsfile')
    <?= Collective\Html\HtmlFacade::script(URL::asset("js/app/recorcv2/userinfo.js")) ?>
@stop
<?php } ?>