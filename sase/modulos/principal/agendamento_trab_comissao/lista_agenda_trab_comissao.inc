
<script>
    function filtraMunicipios(estuf) {
        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {filtroAjax: true, filtroEstado: estuf},
            success: function (resposta) {
                jQuery('#divFiltroMunicipio .inputarea').html(resposta);
            }
        });
    }

    function editar_agenda( muncod ) {
        window.location.href = '/sase/sase.php?modulo=principal/agendamento_trab&acao=A&aba=edit&muncod=' + muncod;
    }
    
    function atualizaComboMunicipio( estuf ){
        $.ajax({
            type    : "POST",
            url     : window.location.href,
            data    : "request=atualizaComboMunicipio&estuf="+estuf,
            async: false,
            success: function(resp){
                $('#div_combo_municipio').html(resp);
                $('.chosen-select').chosen();
            }
        });
    }

    function excluirAssessoramento(assid) {
        if (confirm("Deseja realmente inativar esse Assessoramento?")) {
            window.location.href = '/sase/sase.php?modulo=principal/assessoramento&acao=A&aba=lista&excluir=1&assid=' + assid;
        }
    }
    
    
    function geraRelatorioXls() {
        $('#formulario').val('1');
        $('#funcao').val('xls');
        $('#formulario_pesquisa').submit();
    }
    
    
    function pesquisar_estado_municipio( opc ){
        if( opc == 'T' ){
            $('#estuf').val('');
            $('#muncod').val('');
            $('#acao').val('');
            $('#funcao').val('');
        }else{
            $('#acao').val('');
        }
        $('#formulario_pesquisa').submit();           
    }
    function geraRelatorioXls() {
        $('#funcao').val('xls');
        $('#formulario_pesquisa').submit();
    }
</script>

<br>

<div id="formularioListagem_agenda">
    <form class="form-horizontal" name="formulario_pesquisa" id="formulario_pesquisa" role="form" method="POST">
        <input type="hidden" name="funcao" id="funcao" value=""/>
        <input type="hidden" name="acao" id="acao" value=""/>
        
        <div class="form-group" id="divFiltroEstado">
            <label for="filtroEstado" class="col-sm-2 control-label">Estado:</label>
            <div class="col-sm-10">
                <?php
                    $estuf = $_REQUEST['estuf'];
                    $sql = "
                        SELECT  estuf AS codigo, 
                                estuf ||' - '|| estdescricao AS descricao
                        FROM territorios.estado 
                    ";
                    $db->monta_combo("estuf", $sql, 'S', 'Selecione...', 'atualizaComboMunicipio(this.value)', '', '', '436', 'S', 'estuf', false, $estuf, 'Estado', '', 'chosen-select');
                ?>
            </div>
        </div>
        
        <div class="form-group" id="divFiltroMunicipio">
            <label for="filtroEstado" class="col-sm-2 control-label">Município:</label>
            <div class="col-sm-10" id="div_combo_municipio">
                <?php
                    $muncod = $_REQUEST['muncod'];
                    $sql = "
                        SELECT  muncod AS codigo, 
                                mundescricao AS descricao
                        FROM territorios.municipio 
                    ";
                    $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', '436', 'S', 'muncod', false, $muncod, 'Município', '', 'chosen-select');
                ?>
            </div>
        </div>
        
        <div class="form-group" id="divFiltroEtapas">
            <label for="filtroEstado" class="col-sm-2 control-label">Etapas Preenchidas:</label>
            <div class="col-sm-10">
                <?php
                    $etapas = $_REQUEST['etapas'];
                    $sql = "
                        SELECT  'S' AS codigo, 
                                'Sim' AS descricao
                        UNION
                        SELECT  'N' AS codigo, 
                                'Não' AS descricao
                    ";
                    $db->monta_combo("etapas", $sql, 'S', 'Selecione...', '', '', '', '436', 'S', 'etapas', false, $etapas, 'Etapas Preenchidas', '', 'chosen-select');
                ?>
            </div>
        </div>
        
        <br>

        <div class="form-group">
            <label class="col-sm-5 control-label">&nbsp;</label>
            <div class="col-sm-6">
                <input type="button" class="btn btn-primary" onclick="pesquisar_estado_municipio('P');" value="Pesquisar" />
                &nbsp;&nbsp;
                <input type="button" class="btn btn-primary" onclick="pesquisar_estado_municipio('T');" value="Listar Todos" />
                &nbsp;&nbsp;
                <input type="button" class="btn btn-primary" onclick="geraRelatorioXls()" value="Exportar XLS" />
            </div>
        </div>
        <hr/>
    </form>
    
    <?php
        $_lista = new Sase_Model_Etapaagendacomcoord();
        $_lista->listagemMunicipiosAgenda( $_REQUEST );
        
        if($_POST['funcao']){
            $_lista->montaListaXls( $_POST );
            exit();
        }
    ?>
</div>

<!--<div id="footer"></div>-->