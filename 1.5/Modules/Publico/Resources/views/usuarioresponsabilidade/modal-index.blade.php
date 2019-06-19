@if (!$objContainer->tiposResponsabilidades)
    <div class="form-group">
        <div class="alert alert-warning" >
            <p style="text-align:center">Este sistema não possui responsabilidades a serem atribuídas</p>
        </div>
    </div>
@else
<div class="wrapper wrapper-content">
    
    
    @if ($message = Session::get('success'))
    <div class="alert alert-success">
        <p>{{ $message }}</p>
    </div>
    @elseif($message = Session::get('warning'))
    <div class="alert alert-warning">
        <p>{{ $message }}</p>
    </div>
    @elseif($message = Session::get('error'))
    <div class="alert alert-danger">
        <p>{{ Session::get('error') }}</p>
    </div>
    @endif
    <div class="ibox-content">
        <div class="panel-body">
            <div class="panel-group" id="accordion">
                
                @foreach($objContainer->perfis as $key => $perfil )
                    <div class="panel panel-default">
                        <div class="panel-heading">
                                <table cellpadding="2" cellspacing="0" min-width='98%'>
                                    <tr>
                                        <td width="250px" rowspan="2" align="left" bgcolor="" align="center">&nbsp;</td>
                                        <td width="" align="center" colspan="{{count($objContainer->tiposResponsabilidades[$key])}}" bgcolor="#e9e9e9" align="center" style="border-bottom: 1px solid #bbbbbb">Responsabilidades</td>
                                    </tr>
                                    <tr>
                                        @foreach($objContainer->tiposResponsabilidades[$key] as $respKey => $responsabilidade )
                                            <td align="center" bgcolor="#e9e9e9">{{$responsabilidade->tprdsc}}</td>       
                                        @endforeach
                                    </tr>
                                    <tr>
                                        <td min-width='15%'>
                                            <h5 class="panel-title" >
                                                <a data-toggle="collapse" data-parent="#accordion" href="#collapse{{$key}}" class="" aria-expanded="true">{{ $perfil }}</a>
                                            </h5>
                                        </td>
                                        @foreach($objContainer->tiposResponsabilidades[$key] as $respKey => $responsabilidade )
                                            <td align="center" width='20%'>
                                                @if (!is_null($responsabilidade->tprcod) && $responsabilidade->pflcod == $key) 
                                                    <input type="button" pflcod="{{$key}}" tprcod="{{$responsabilidade->tprcod}}" respKey="{{$respKey}}" trpcod="{{$responsabilidade->trpcod}}" id="btnAbrirResp{{$key}}_{{$responsabilidade->tprcod}}" name="btnAbrirResp{{$responsabilidade->pflcod}}" class="btn btn-success btnAbrirResp" value="Atribuir" >
                                                @else 
                                                    -
                                                @endif
                                            </td>
                                        @endforeach
                                    </tr>
                                </table>
                            
                        </div>
                        <div id="collapse{{$key}}" class="panel-collapse collapse">
                            <div class="panel-body">
                                @foreach($objContainer->tiposResponsabilidades[$key] as $respKey => $responsabilidade )                                
                                    @if (!is_null($responsabilidade->tprcod) && $responsabilidade->pflcod == $key)
                                        <table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
                                            <tr>
                                                <td colspan="3">{{$responsabilidade->tprdsc}}</td>
                                            </tr>
                                            <tr style="color:#000000;">
                                                <td valign="top" width="12">&nbsp;</td>
                                                <td valign="top">Código</td>
                                                <td valign="top">Descrição</td>
                                            </tr>
                                            @foreach($objContainer->UsuarioResponsabilidades[$key][$responsabilidade->tprcod] as $usrRespKey => $usrResp )
                                                <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='F7F7F7';" bgcolor="F7F7F7">
                                                    <td valign="top" width="12">&nbsp;</td>
                                                    <td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap>{{$usrResp->codigo}}</td>
                                                    <td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;">{{$usrResp->descricao}}</td>
                                                </tr> 
                                            @endforeach
                                            <tr>
                                                <td colspan="4" align="right" style="color:000000;border-top: 2px solid #000000;">
                                                  Total: ({{count($objContainer->UsuarioResponsabilidades[$key][$responsabilidade->tprcod])}})
                                                </td>
                                            </tr>
                                        </table>
                                    @endif
                                @endforeach
                            </div>
                        </div>
                    </div>
                @endforeach 
            </div>
        </div>
    </div>
</div>

<div class="modal fade" id="atribuirModal" tabindex="-1" role="dialog" aria-labelledby="atribuirModalLabel" aria-hidden="true">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title" id="atribuirModalLabel">Atribuição de Responsabilidades</h5>
        <button type="button" class="close modalResponsabilidade"  aria-label="Close">
          <span aria-hidden="true">&times;</span>
        </button>
      </div>
      <div class="modal-body" id="bodyModal">
            <div class="row">
                <div class="col-lg-12">
                    <div class="table-responsive"> 
                        <table id="tb_responsabilidades" class="table table-hover table-condensed table-bordered" style="width:100%">        
                            <thead>
                                <tr>
                                    <th class="text-center"><!--<input type="checkbox" id="checkAll"/>--></th>                                
                                    <th class="text-center">Código</th>
                                    <th class="text-center">Descrição</th>
                                </tr>
                            </thead>

                        </table>  
                    </div>
                </div>
        </div>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-secondary modalResponsabilidade" >Close</button>
<!--        <button type="button" class="btn btn-primary">Save changes</button>-->
      </div>
    </div>
  </div>
</div>
@endif



    <script>
        var dataTable_;
        
        function atualizaTabelaResponsabilidadesUsuario () {
            $.ajax({
                method: "POST",
                url: "{{ route('usuarioresponsabilidade.geraTabelaResponsabilidadesUsuario') }}",
                data: { 
                        usucpf: '{{$objContainer->usucpf}}'
                      }
            }).done(function( result ) {
                //console.log(result[1024]);
                var index;
                for(index in result) {
                    $("#collapse"+index).html('');
                    $("#collapse"+index).html(result[index]);
               }
            });
        }
        
        function GeraTabelaResponsabilidades (objButton) {
            var parametros  = JSON.parse('{!! json_encode($objContainer) !!}');

                //Ação - Ao selecionar o Tipo Resposabilidade.
                if ($.fn.DataTable.isDataTable('#tb_responsabilidades')) {
                    dataTable_.clear();
                    dataTable_.destroy();
                }

                dataTable_ =  $('#tb_responsabilidades').DataTable({
                    "processing": true,
                    "serverSide": true,
                    "ajax": {
                        url: "{{ route('usuarioresponsabilidade.getList') }}",
                        method: 'POST',
                        data:{ pflcod : $(objButton).attr('pflcod'),
                               tprcod : $(objButton).attr('tprcod'),
                               trpcod : $(objButton).attr('trpcod'),
                               respKey : $(objButton).attr('respKey'),
                               usucpf : '{{$objContainer->usucpf}}',
                               tprjoincolumn : parametros.tiposResponsabilidades[$(objButton).attr('pflcod')][$(objButton).attr('respKey')]['tpr_join_column'],
                               tprjoindesc : parametros.tiposResponsabilidades[$(objButton).attr('pflcod')][$(objButton).attr('respKey')]['tpr_join_table_column_dsc'],
                               tprjointable : parametros.tiposResponsabilidades[$(objButton).attr('pflcod')][$(objButton).attr('respKey')]['tpr_join_table'],
                               tprjoinschema : parametros.tiposResponsabilidades[$(objButton).attr('pflcod')][$(objButton).attr('respKey')]['tpr_join_schema'],
                               tprjointablecolumnstatus : parametros.tiposResponsabilidades[$(objButton).attr('pflcod')][$(objButton).attr('respKey')]['tpr_join_table_column_status'],
                            }

                    },
                    columnDefs: [
                                { data: 'action', name: 'edit', orderable: false, searchable: false, targets: 0, visible: true},
                                { data: 'codigo', name: 'codigo' , targets: 1, visible: true },
                                { data: 'descricao', name: 'descricao', targets: 2, visible: true },
                                {
                                    "targets": 0,
                                    "orderable": false,
                                    "searchable": false,
                                },
                            ]
                });
        }
        
        $(document).ready(function () {
            $(".btnAbrirResp").click(function () {
                GeraTabelaResponsabilidades($(this));
                
                $("#atribuirModal").modal({
                    show: 'true'
                });
                //alert($(this).val());
            });
            
            $(".modalResponsabilidade").click(function(){
                $("#atribuirModal").modal('hide');
            });
            
            //Ação da seleção do checkbox do datatable
            $('#tb_responsabilidades').on('click', 'input[type="checkbox"]', function() {
                var btnAtt = $(this).attr('btnAtt');
                var trpcod =  $(this).attr('trpcod');
                var responsabilidadecod = $(this).val();
                    $.ajax({
                       method: "POST",
                       url: "{{ route('usuarioresponsabilidade.associarResponsabilidade') }}",
                       data: { trpcod: trpcod, 
                               responsabilidadecod: responsabilidadecod, 
                               usucpf: '{{$objContainer->usucpf}}'
                             }
                    }).done(function( msg ) {
                        atualizaTabelaResponsabilidadesUsuario();
                        GeraTabelaResponsabilidades($("#"+btnAtt));
                    });
            });

        });
        
    </script>

    <!--<script src="{!! asset('js/plugins/dataTables/datatables.min.js') !!}"></script>-->
