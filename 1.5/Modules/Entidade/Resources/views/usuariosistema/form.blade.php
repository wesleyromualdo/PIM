<div class="row">
    <div class="col-lg-6" style="border: 0px solid activecaption;">
        <div class="ibox" style="border: 0px solid red;">
            <div class="ibox-content text-center p-md">
                <div class="row">                
                    <div class="form-group {{ $errors->has('suscod') ? 'has-error' : '' }}">
                        <label class="col-md-3 control-label text-right p-xxs" for="suscod">Status:</label>
                         <div class="col-md-9">

                            <section class="btn-group" data-toggle="buttons">                   
                                <label id="label_suscod_A" for="status_ativo" class="btn btn-default labelsuscod">
                                    <input id="suscod_A" type="radio" name="suscod" value="A" >
                                    Ativo
                                </label>
                                <label id="label_suscod_P" for="status_pendente" class="btn btn-default labelsuscod">
                                    <input id="suscod_P" type="radio" name="suscod" value="P">
                                    Pendente
                                </label>
                                <label id="label_suscod_B" for="status_bloqueado" class="btn btn-default labelsuscod">
                                    <input id="suscod_B" type="radio" name="suscod" value="B">
                                    Bloqueado
                                </label>
                            </section>
                            <span class="text-danger" id="erroSpan_suscod">{{ $errors->first('suscod','') }}</span>
                        </div>
                    </div>
                </div>
                <div class="form-group {{ $errors->has('htudsc') ? 'has-error' : '' }}">
                    <div class="row" style="display: none;" id="row_htudsc">
                        <label class="col-md-3 control-label text-right p-xxs">Justificativa:</label>
                         <div class="col-md-9">
                             {!! Form::textarea('htudsc', '', array('class' => 'form-control', 'rows' => 4, 'id' => 'htudsc', 'disable' => 'disabled')) !!}
                            <span class="text-danger">{{ $errors->first('htudsc','') }}</span>
                        </div>
                    </div>
                </div>
                <div class="form-group {{ $errors->has('pflcod') ? 'has-error' : '' }}">
                    <div class="row">
                        <label class="col-md-3 control-label text-right p-xxs" for="perfil">Perfil:</label>
                        <div class="col-md-9">
                            <select class="form-control" data-placeholder="Selecione .." id="pflcod" multiple name="pflcod[]">                
                            </select>
                            <span class="text-danger" id="erroSpan_pflcod">{{ $errors->first('pflcod','') }}</span>
                        </div>
                    </div> 
                </div>
                <input type="hidden" id="sisid" name="sisid">
                <input type="hidden" id="usucpf" name="usucpf" value="{{$usuario->usucpf}}">
            </div>
        </div>
    </div>
    <div class="col-lg-6" style="border: 0px solid activecaption; overflow: auto; max-height: 250px">
        <div class="ibox" style="border: 0px solid red;">
            <div class="ibox-title" style="width: 30%">
                <h5>Histórico</h5>
<!--                <span class="label label-primary">Meeting today</span>-->
                <div class="ibox-tools">
                    <a class="collapse-link">
                        <i class="fa fa-chevron-up"></i>
                    </a>
<!--                    <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                        <i class="fa fa-wrench"></i>
                    </a>
                    <ul class="dropdown-menu dropdown-user">
                        <li><a href="#" class="dropdown-item">Config option 1</a>
                        </li>
                        <li><a href="#" class="dropdown-item">Config option 2</a>
                        </li>
                    </ul>
                    <a class="close-link">
                        <i class="fa fa-times"></i>
                    </a>-->
                </div>
            </div>
            <div class="ibox-content text-center inspinia-timeline" >
                <div class="timeline-item">
                    <div class="row">
                        <table class="display table table-hover table-condensed table-bordered stripe compact dataTable no-footer">
                            <thead style="font-size: 12px;">
                                <tr>
                                    <th class="text-center">Data</th>
                                    <th class="text-center">Status</th>
                                    <th class="text-center">Descrição</th>
                                    <th class="text-center">CPF</th>
                                </tr>
                            </thead>
                            <tbody id="tbodyHistorico" style="font-size: 12px;">
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

@section('myjsfile')
    <script language="javascript">
        var suscod_old;

        function AtualizaCamposForm (sisid, suscod) {
            GeraMultiSelectPerfil(sisid);
            GetHistoricoUsuario(sisid);
            $("#suscod_"+suscod).attr('checked',true);
            $("#label_suscod_"+suscod).addClass('active');
        }

        function GeraMultiSelectPerfil(sisid) {
            $("#pflcod").chosen();
            $("#pflcod").chosen('destroy');
            $.ajax({
                type: "POST",
                url: "{{ route('usuariosistema.pegaPerfisSistemabyUsuario') }}",
                data: {
                    sisid: sisid,
                    usucpf: '{{$usuario->usucpf}}'
                },
                success: function(result){
                    //console.log(result);
                    var index;
                    var opt = '';
                    $("#pflcod").html('');
                    for(index in result) {
                        selected = '';
                        if (result[index]['usucpf'] != null) {
                            selected = 'selected';
                        }

                        opt += '<option value="'+result[index]['pflcod']+'" '+selected+'>'+result[index]['pfldsc']+'</option>';
                    }
                    $("#pflcod").html(opt);
                    $("#pflcod").chosen({width: "100%"});
                    $(".chosen-drop").css('position', 'relative');
                }, 
                error: function(XMLHttpRequest, textStatus, errorThrown) {
                    console.log(XMLHttpRequest);
                }
            });
        }

        function GetHistoricoUsuario(sisid) {
            $.ajax({
                type: "POST",

                url: "{{ route('usuariosistema.getHistorico') }}",
                data: {
                    sisid: sisid,
                    usucpf: '{{$usuario->usucpf}}'
                },
                success: function(result){
                    //console.log(result);
                    $("#tbodyHistorico").html('');
                    if (result) {
                        var index;
                        var opt = '';
                        var cpfadm;
                        for(index in result) {
                            cpfadm = (result[index]['usucpfadm'] ? result[index]['usucpfadm'] : '');
                            opt += '<tr><td>'+result[index]['htudata']+'</td>'+
                                   '<td>'+result[index]['suscod']+'</td>'+
                                   '<td>'+result[index]['htudsc']+'</td>'+
                                   '<td>'+cpfadm+'</td></tr>';
                        }
                        $("#tbodyHistorico").html(opt);
                    }
                }, 
                error: function(XMLHttpRequest, textStatus, errorThrown) {
                    console.log(XMLHttpRequest);
                }
            });
        }

        function SubmitForm() {
            $.ajax({
                type: "POST",
                //'.usuariosistema.store'
                url: "{{ route('seguranca.usuariosistema.store') }}",
                data: {
                    sisid: $("#sisid").val(),
                    usucpf: '{{$usuario->usucpf}}',
                    pflcod: $("#pflcod").val(),
                    htudsc: $("#htudsc").val(),
                    suscod: $('input[name=suscod]:checked', '#formUsuarioSistema').val()

                },
                success: function(result){
                    //console.log(result);                    
                    alert(result['msg']);
                    GetHistoricoUsuario($("#sisid").val());                    
                }, 
                error: function(XMLHttpRequest, textStatus, errorThrown) {
                    console.log(XMLHttpRequest);
                }
            });
                        
        }


        $(document).ready(function (){
            $("#btnFormUsuarioSistema").click(function(){
                if ($('input[name=suscod]:checked', '#formUsuarioSistema').val() == null) {
                    $("#erroSpan_suscod").html("O campo status é obrigatório.");
                } else if ($("#pflcod").val() == '') {
                    $("#erroSpan_pflcod").html("O campo perfil é obrigatório.");
                    //alert('É necessário preencher o campo status e selecionar ao menos um perfil!');
                } else {
                    SubmitForm();
//                    $("#formUsuarioSistema").submit();
                }
            });

            $(".labelsuscod").click(function(){
                if ($(this).children().val() !== suscod_old) {
                    $("#row_htudsc").show();
                    $("#htudsc").attr('disable', false);
                } else {
                    $("#row_htudsc").hide();
                    $("#htudsc").attr('disable', true);
                }
            });
        });
    </script> 
    @parent
@endsection