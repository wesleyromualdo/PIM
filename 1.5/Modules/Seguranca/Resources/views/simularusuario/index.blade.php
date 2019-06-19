@inject('service', 'Modules\Seguranca\Http\Controllers\UsuarioController')
@php

        $objContainer = $service->index();      
@endphp
<div class="wrapper wrapper-content">
    <div class="ibox-content">
        <div class="row">
            <div class="form-group {{ $errors->has('simula_usucpf') ? 'has-error' : '' }}">
                <label class="col-md-6 control-label text-left p-xxs" for="simula_usucpf">CPF (ou parte do CPF):</label>
                <div class="col-md-6">
                    {!! Form::text('simula_usucpf', null, array('maxlength'=>14,'placeholder' => 'CPF', 'class' => 'form-control numCPF' , 'id'=>'simula_usucpf')) !!}
                    <span class="text-danger">{{ $errors->first('simula_usucpf','') }}</span>
                </div>
            </div>
            <div class="form-group {{ $errors->has('simula_usunome') ? 'has-error' : '' }}">
                <label class="col-md-6 control-label text-left p-xxs" for="simula_usunome">Nome completo (ou parte do nome):</label>
                 <div class="col-md-6">
                    {!! Form::text('simula_usunome', null, array('maxlength'=>100,'placeholder' => 'Nome completo','class' => 'form-control', 'id'=>'simula_usunome')) !!}
                    <span class="text-danger">{{ $errors->first('simula_usunome','') }}</span>
                </div>
            </div>
            <div class="form-group {{ $errors->has('simula_pflcod') ? 'has-error' : '' }}">
                <label class="col-md-6 control-label text-left p-xxs" for="simula_pflcod">Perfil:</label>
                 <div class="col-md-6">
                    {!! Form::select('simula_pflcod', $objContainer->perfis, null, ['class' => 'form-control', 'placeholder' => 'Selecione...', 'id' => 'simula_pflcod'])!!}
                </div>
            </div>            
            <div class="form-group">
                <div class="col-sm-5 col-sm-offset-3">
                    <br>
                    <button type="button" class="btn btn-default" data-dismiss="modal"><i class="fa fa-arrow-left"></i>Voltar</button>
                    <button type="button" class="btn btn-primary" id="btn-pesquisar-simula-usuario"><i class="fa fa-search"></i> Pesquisar</button>
                </div>
            </div>                         
        </div>            
    </div>
</div>
<div class="wrapper wrapper-content">
        <div class="ibox-content">
            <div class="row">                                             
                <div class="col-lg-12">
                   <div class="table-responsive"> 
                       <table id="tbSimulaUsuario" class="table table-hover table-condensed table-bordered" style="width:100%">        
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
    </div>

    <script src="{!! asset('js/jquery-3.1.1.min.js') !!}"></script>
    <script src="{!! asset('js/plugins/dataTables/datatables.min.js') !!}"></script>
    <script>
        var SimulaDataTable_;
        
        
        function GeraTabelaSimularUsuario () {
            //var parametros  = JSON.parse('{!! json_encode($objContainer) !!}');

                //Ação - Ao selecionar o Tipo Resposabilidade.           
                if ($.fn.DataTable.isDataTable('#tbSimulaUsuario')) {                
                    SimulaDataTable_.clear();
                    SimulaDataTable_.destroy();
                }

                SimulaDataTable_ =  $('#tbSimulaUsuario').DataTable({
                    "processing": true,
                    "serverSide": true,
                    "ajax": {
                        url: "{{ route('usuario.GeraTabelaSimularUsuario') }}",
                        method: 'POST',
                        data:{ usucpf : $("#simula_usucpf").val(),
                               usunome : $("#simula_usunome").val(),
                               pflcod : $("#simula_pflcod").val(),
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
            
            $("#btn-pesquisar-simula-usuario").click(function () {
                GeraTabelaSimularUsuario();
            });
            
            //Ação da seleção do checkbox do datatable
            $('#tbSimulaUsuario').on('click', 'input[type="radio"]', function() {
                var usucpf = $(this).val();
                    $.ajax({
                       method: "POST",
                       url: "{{ route('usuario.SimulaUsuario') }}",
                       data: {  
                               usucpf: usucpf
                             }
                    }).done(function( msg ) {
                        console.log(msg);
                        location.reload();
                    });
            });      

            $('#btnVoltarOrigemUsuario , #btnVoltarOrigemUsuario i').on('click', function() {
                    $.ajax({
                       method: "POST",
                       url: "{{ route('usuario.VoltaUsuario') }}",
                       data: {  
                               usucpf: "{{$_SESSION['usucpforigem']}}",
                             }
                    }).done(function( msg ) {
                        location.reload();
                    });
            });
        });
        
    </script>
    
