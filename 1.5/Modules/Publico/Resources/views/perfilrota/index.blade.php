@extends('layouts.app')
@section('title', 'Listar perfis rotas')
@section('content')
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Gerenciar Perfis Rota</h2>
        <ol class="breadcrumb">
            <li><a href="#">Sistema</a></li>
            <li>Tabelas de Apoio</li>
            <li class="active"><strong>Gerenciar Perfil rota</strong></li>
        </ol>
    </div>
</div>
<div class="wrapper wrapper-content">
    @if ($message = Session::get('success'))
    <div class="alert alert-success">
        <p>{{ $message }}</p>
    </div>
    @endif
    <div class="ibox-content">

        {!! Form::open(array('route' => '.perfilrota.store','method'=>'POST', 'class' => 'form-horizontal')) !!}

        <div class="row">
            <div class="form-group">
            <label class="col-md-2 control-label text-right p-xxs" for="sistemas">Sistemas:</label>
            <div class="col-md-9">
                {!! Form::select('sistemas', $sistemas, null, ['class' => 'form-control', 'placeholder' => 'Selecione...', 'id' => 'sistemas'])!!}
            </div>
        </div>

            <div class="form-group">
            <label class="col-md-2 control-label text-right p-xxs" for="perfis">Perfil:</label>
            <div class="col-md-9">
                <select id="perfis" name="perfis" class="form-control"><option>Selecione...</option></select>
            </div>
            </div>

            <div class="col-lg-5"></div>
            <div class="col-lg-3 text-right">
            &nbsp;
            </div>
        </div>
        <hr>
        <div class="row">
            <div class="col-lg-12">
                <div class="table-responsive"> 
                    <table id="tb_perfis_rota" class="table table-hover table-condensed table-bordered" style="width:100%">
                        <thead>
                            <tr>
                                <th class="text-center"><input type="checkbox" id="checkAll"/></th>
                                <th class="text-center">Rota</th>
                                <th class="text-center">Rota Descriçao</th>
                                <th class="text-center">Sisid</th>
                            </tr>
                        </thead>            
                        
                    </table>  
                </div>
            </div>
        </div>

        {!! Form::close() !!}

    </div>
</div>
@endsection

@section('myjsfile')
    <script>
        var dataTable_;

        /*acao de selecao*/
        $("#tb_perfis_rota").on('click','input[type="checkbox"]',function () {
            var pflcod =  $('#perfis').val();
            var co_rota =$(this).attr('co_rota');
            var sisid =  $("#sistemas").val();
            var checkedAll = $("#checkAll:checked").length;
            associaRota(pflcod,sisid,co_rota,checkedAll);
        });


        function associaRota(perfil,sisid,rota,checkall){
            $.ajax({
                method: "POST",
                url: "{{ route('perfilrota.associarRota') }}",
                data: {
                    pflcod: perfil,
                    sisid: sisid,
                    co_rota: rota,
                    checkall: checkall
                }
            }).done(function( msg ) {
                console.log( "Registro atualizado: " + msg );
            });
        }

        //Checkboxes - Marcar todos.
        $('#checkAll').click(function(){
            $(".chkPerfilRota").prop('checked', $(this).prop('checked'));
        });

        //A cada paginação volta o checkbox do header para unchecked.
        $('#tb_perfis_rota').on( 'page.dt', function () {
            $("#checkAll").prop('checked', false);
        });




        /*recupera as rotas do perfil*/
        $('#perfis').on('change',function () {
            var sisid =  $('#sistemas').val();
            var pflcod = $('#perfis').val();
            if ($.fn.DataTable.isDataTable('#tb_perfis_rota')) {
                dataTable_.clear();
                dataTable_.destroy();
            }
            dataTable_ =  $('#tb_perfis_rota').DataTable({
                "processing": true,
                "serverSide": true,
                "paging":   false,
                "ajax": {
                    url: "{{ route('perfilrota.getList') }}",
                    data: function (params) {
                        params.pflcod = pflcod;
                        params.sisid = sisid;
                    },
                },
                columnDefs: [
                    { data: 'action', name: 'edit', orderable: false, searchable: false, targets: 0, visible: true},
                    { data: 'ds_rota', name: 'ds_rota', targets: 1, visible: true },
                    { data: 'ds_rota_descricao', name: 'ds_rota_descricao', targets: 2, visible: true },
                    { data: 'sisid', name: 'sisid', targets: 3, visible: true },
                    {
                        "targets": 0,
                        "orderable": false,
                        "searchable": false,
                    },
                ]
            });

        });

        function setCMBPerfis(perfis){
            var select = $('#perfis');
            select.find('option').remove();
            select.append(new Option("Selecione...", "0"));
            $.each(perfis, function(key,value) {
                $('<option>').val(key).text(value).appendTo(select);
            });

        }
        /* recupera os perfis do sistema*/
        $('#sistemas').on('change', function(){

            var sisid =  $('#sistemas').val();
            if ($.fn.DataTable.isDataTable('#tb_perfis_rota')) {
                dataTable_.clear();
                dataTable_.destroy();
            }

            $.ajax({
                type: "POST",
                url: "{{ route('perfilrota.getPerfil') }}",
                data: {sisid:sisid},
                success: function(res){
                    console.log(res);
                    setCMBPerfis(res);
                },
                error: function(XMLHttpRequest, textStatus, errorThrown) {
                    console.log(XMLHttpRequest);
                }
            });
        });

    </script>

    <script src="{!! asset('js/plugins/dataTables/datatables.min.js') !!}"></script>
@stop