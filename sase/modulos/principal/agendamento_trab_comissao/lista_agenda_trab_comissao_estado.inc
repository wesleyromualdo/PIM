<script>

    function editar_agenda( estuf ) {
        window.location.href = '/sase/sase.php?modulo=principal/agendamento_trab_estado&acao=A&aba=edit_estado&estuf=' + estuf;
    }
    
    function excluirAgendamento(aceid) {
        if (confirm("Deseja realmente inativar esse Agendamento?")) {
            window.location.href = '/sase/sase.php?modulo=principal/agendamento_trab_estado&acao=A&aba=lista_estado&excluir=1&aceid=' + aceid;
        }
    }
    
    
    function geraRelatorioXls() {
        $('#acao').val('xls');
        $('#formulario_pesquisa').submit();
    }
    
    
    function pesquisar_estado( opc ){
        if( opc == 'T' ){
            $('#estuf').val('');
            $('#acao').val('');
            $('#funcao').val('');
        }else{
            $('#acao').val('');
        }
        $('#formulario_pesquisa').submit();           
    }
</script>

<br>
<div id="formularioListagem_agenda">
    <form class="form-horizontal" name="formulario_pesquisa" id="formulario_pesquisa" role="form" method="POST">
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
                    $db->monta_combo("estuf", $sql, 'S', 'Selecione...', '', '', '', '436', 'S', 'estuf', false, $estuf, 'Estado', '', 'chosen-select');
                ?>
            </div>
        </div>
        
        <div class="form-group" id="divFiltroPreenchido">
            <label for="filtroPreenchido" class="col-sm-2 control-label">Etapas Preenchidas:</label>
            <div class="col-sm-10">
                <?php
                    $preenchido = $_REQUEST['preenchido'];
                    $sql = "
                        SELECT  'S' AS codigo, 
                                'Sim' AS descricao
                        UNION
                        SELECT  'N' AS codigo, 
                                'Não' AS descricao
                    ";
                    $db->monta_combo("preenchido", $sql, 'S', 'Selecione...', '', '', '', '436', 'S', 'preenchido', false, $preenchido, 'Etapas Preenchidas', '', 'chosen-select');
                ?>
            </div>
        </div>
        
        <br>

        <div class="form-group">
            <label class="col-sm-5 control-label">&nbsp;</label>
            <div class="col-sm-6">
                <input type="button" class="btn btn-primary" onclick="pesquisar_estado('P');" value="Pesquisar" />
                &nbsp;&nbsp;
                <input type="button" class="btn btn-primary" onclick="pesquisar_estado('T','');" value="Listar Todos" />
                 &nbsp;&nbsp;
                <input type="button" class="btn btn-primary" onclick="geraRelatorioXls()" value="Exportar XLS" />
            </div>
        </div>
        <hr/>
    </form>
    <?php
    $_lista = new Agendacomissaocoordestado();
    $_lista->listagemEstadoAgenda( $_REQUEST );
        
    if($_POST['acao']){
        $_lista->montaListaXls( $_POST );
        exit();
    }
    ?>
</div>

<!--<div id="footer"></div>-->