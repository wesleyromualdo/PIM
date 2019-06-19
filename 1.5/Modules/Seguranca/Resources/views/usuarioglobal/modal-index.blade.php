<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="lightBoxGallery" style="text-align: left">
            <table id="tb_usuariosistema" class="table table-hover table-condensed table-bordered stripe" style="width:100%">        
                <thead>
                    <tr>                                                        
                        <th class="text-center">Código</th>
                        <th class="text-center">Descrição</th>
                        <th class="text-center">Diretório</th>
                        <th class="text-center">Status Usuário</th>
                    </tr>
                </thead>            

            </table>   
            
        </div>
    </div>
</div>

<div class="modal fade" id="UsrSisModal" tabindex="-1" role="dialog" aria-labelledby="UsrSisModallLabel" aria-hidden="true">
  <div class="modal-dialog modal-lg" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title" id="atribuirModalLabel">Atribuição de Sistema</h5>
        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
          <span aria-hidden="true">&times;</span>
        </button>
      </div>
        <div class="modal-body" id="bodyModal" style="overflow: auto; max-height: 600px; padding: 0px 0px 0px 0px;">
            <div class="tabs-container">
                    <ul class="nav nav-tabs" role="tablist">
                        <li class="active">
                            <a class="nav-link active" data-toggle="tab" href="#sistemas">Vincular Usuário Sistema</a>
                        </li>
                        <li>
                            <a class="nav-link" data-toggle="tab" href="#responsabilidade">Usuário Responsabilidade</a>
                        </li>
                    </ul>
                    <div class="tab-content">
                        <div role="tabpanel" id="sistemas" class="tab-pane active">
                            <div class="panel-body">
                                <div class="ibox-content">
                                    <div class="row">
                                        <div class="col-lg-12">
                                                        {!! Form::model($usuario, ['method' => 'POST', 'route' => ['.usuariosistema.store'], 'class' => '', 'id' => 'formUsuarioSistema']) !!}
                                                            @include('seguranca::usuariosistema.form', ['errors' => $errors])
                                                        {!! Form::close() !!}
                                            <div class="row" style="padding-top: 20px; display: flex; flex-direction: row; justify-content: center; align-items: center">
                                                <div class="form-group" >
                                                    <div class="col-sm-5 col-sm-offset-2">
                                                        <!--<a href="{{ route('perfil.index') }}" class="btn btn-default"><i class="fa fa-arrow-left"></i> Voltar</a>-->
                                                        <!--<button type="button" class="btn btn-primary" id="btn-pesquisar-usuario"><i class="fa fa-search"></i> Pesquisar</button>-->
                                                        <button type="button" id="btnFormUsuarioSistema" class="btn btn-primary"><i class="fa fa-save"></i> Salvar</button>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                    </div>

                                </div>
                            </div>
                        </div>
                        <div role="tabpanel" id="responsabilidade" class="tab-pane">
                            <div class="panel-body">
                                <div class="ibox-content">
                                    <div class="row">
                                        <div class="col-lg-12" id="bodyResponsabilidades">
                                            
                                        </div>
                                    </div>

                                </div>
                            </div>
                        </div>
                    </div>


                </div>

      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>
<!--        <button type="button" class="btn btn-primary">Save changes</button>-->
      </div>
    </div>
  </div>
</div>



@section('myjsfile')
    <script language="javascript"> 
    var dataTable_usuarioSistema;
    
    function GeraTabelaSistemas () {
        var parametros  = JSON.parse('{!! json_encode($objContainer) !!}');

            //Ação - Ao selecionar o Tipo Resposabilidade.
            if ($.fn.DataTable.isDataTable('#tb_usuariosistema')) {
                dataTable_usuarioSistema.clear();
                dataTable_usuarioSistema.destroy();
            }

            dataTable_usuarioSistema =  $('#tb_usuariosistema').DataTable({
                "processing": true,
                "serverSide": true,
                "paging":false,
                "ajax": {
                    url: "{{ route('usuariosistema.getList') }}",
                    method: 'POST',
                    data:{ //action: 'getList',
                           usucpf: '{{$usuario->usucpf}}',
                        }

                },
                columnDefs: [
                            { data: 'sisid', name: 'sisid' , targets: 0, visible: true },
                            { data: 'sisdsc', name: 'sisdsc', targets: 1, visible: true },
                            { data: 'sisdiretorio', name: 'sisdiretorio', targets: 2, visible: true },
                            { data: 'suscod', name: 'suscod', orderable: true, searchable: false, targets: 3, visible: true, 
                              render: function (val, type, row) {
                                  var label = '';
                                    switch (val) {
                                        case 'A':
                                            label =  'Ativo';
                                            break;
                                        case 'P':
                                            label =  'Pendente';
                                            break;
                                        case 'B':
                                            label =  'Bloqueado';
                                            break;
                                        default :
                                            label = 'Sem Status';
                                            break;
                                    }
                                    return label;
                              }
                            },
                            
                        ]
            });
    }
    
    function CarregaViewUsuarioResponsabilidade(sisid) {
        $("#pflcod").chosen();
        $("#pflcod").chosen('destroy');
        $.ajax({
            type: "POST",
            url: "{{ route('usuarioresponsabilidade.renderView') }}",
            data: {
                sisid: sisid,
                usucpf: '{{$usuario->usucpf}}'
            },
            success: function(result){
                //console.log(result);
                $("#bodyResponsabilidades").html(result);
            }, 
            error: function(XMLHttpRequest, textStatus, errorThrown) {
                console.log(XMLHttpRequest);
            }
        });
    }

    $(document).ready(function() {
        GeraTabelaSistemas();
        
        $("#UsrSisModal").on('hide.bs.modal', function(e){
            $('[name=suscod]').attr('checked', false);
            $('.labelsuscod').removeClass('focus active');
            $("#row_htudsc").hide();
            $("#htudsc").attr('disable', true);
        });
        
        $('#tb_usuariosistema tbody').on('click', 'tr', function() {
            var data = dataTable_usuarioSistema.row( this ).data();
//            console.log(data);

            AtualizaCamposForm(data['sisid'], data['suscod']);
            CarregaViewUsuarioResponsabilidade(data['sisid']);
            
            $("#sisid").val(data['sisid']);
            suscod_old =  data['suscod'];
            
            //alert("click no sistema: <b>'"+data['suscod']+"'</b>");
            $("#UsrSisModal").modal({
                show: 'true'
            });
        });
    });
</script> 
@parent
@endsection