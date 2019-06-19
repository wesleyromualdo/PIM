<?php
//$arraycpfs = array(, , );

if( possui_perfil(array(PFLCOD_SUPER_USUARIO)) || in_array($_SESSION['usucpf'],$arraycpfs) ){
       
    //Chamada de programa
    include_once APPRAIZ . "obras2/classes/modelo/Avisoobras.inc";
    include_once APPRAIZ . "obras2/classes/modelo/Anexoaviso.inc";
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    //id acao alterar
    $avsoid   = $_GET['avsoid'];
    //$hab      = $avsoid ? "N" : '';

    $aviso = new Obras2_Modelo_Avisoobras($avsoid);

    if ( $aviso->avsoid ){
        extract( $aviso->getDados() );
    }

    if(!empty($_REQUEST['tramitacao'])){
        $requisicao = $_REQUEST['tramitacao'];
        switch ($requisicao) {
            case 'excluirFile'://opção excluir tela editar
                $arqid = $_REQUEST['excluirFile'];
                if($arqid){
                    $file = new FilesSimec("anexoaviso", array(''), "obras2");
                    $booResultado = $file->setRemoveUpload( $arqid );
                    if($booResultado){
                       echo "<script>alert('Arquivo excluído com sucesso');</script>";
                    }else{
                         echo "<script> alert('Erro ao Excluir arquivo');</script>";
                    }
                    header("location:obras2.php?modulo=principal/editarAviso&acao=A");
                }
            break;
            case 'excluirAviso'://opção excluir registro na lista
                if(!empty($_POST['excluirAviso'])){

                    if(!$anexoAviso){
                        $anexoAviso = new Obras2_Modelo_Anexoaviso();
                    }
                    //primeiro deletar os arquivos antes de excluir dados do Aviso
                    $anexoAviso->deletarFilePorAvsoid($_POST['excluirAviso']);
                    $aviso->excluirAviso($_POST['excluirAviso']);
                }else{
                    $fm->addMensagem('Erro ao encontrar o arquivo');
                }
            break;
            case 'salvar':
                //validando se campo ordem foi preenchido
                $avsoordem = $_REQUEST['aviso']['avsoordem'] ? $aviso->ordemExiste($_REQUEST['aviso']['avsoordem']) : '';
                //verificando se já existe o número de ordem informado e não é alteração.
                if(!empty($avsoordem) && empty($avsoid)){
                     echo "<script> alert('O número da ordem já existe!');</script>";
                     echo "<script> 
                                window.onload = function(){ focusOrdem(); return false;};
                           </script>";
                }else{
                    $_REQUEST['aviso']['avsovalidade']  = formata_data_sql($_REQUEST['aviso']['avsovalidade']);

                    $aviso->popularDadosObjeto($_REQUEST['aviso']);
                    $idAviso = $aviso->salvar();
                    
                    //recuperando id após salvar aviso
                    if($idAviso){
                        
                        //caso update para arquivo existente, alterar apenas descrição do arquivo
                        if($_REQUEST['arquivo'][0] != '' && empty($_FILES['arquivo']['name'][0]) ){
                            foreach($_REQUEST['arquivo'] as $a => $aarqid){
                                if(!$anexoAviso){
                                    $anexoAviso = new Obras2_Modelo_Anexoaviso();
                                }
                                $anexoAviso->updateDescricaoFile($_REQUEST['anxavdsc'][$a], $aarqid);
                            }
                        }elseif($_FILES['arquivo']['name'][0] != ""){
                            
                            //salvar dados na tabela public.arquivos
                            $arquivos = $_FILES;
                            foreach ($arquivos['arquivo']['name'] as $key => $value) {
                                if(empty($value))
                                    continue;
                                
                                //tratando dados para popular o FILE, por ser um array de arquivos
                                $files = array(
                                        'arquivo' => array(
                                        'name' => $arquivos['arquivo']['name'][$key],
                                        'type' => $arquivos['arquivo']['type'][$key],
                                        'tmp_name' => $arquivos['arquivo']['tmp_name'][$key],
                                        'error' => $arquivos['arquivo']['error'][$key],
                                        'size' => $arquivos['arquivo']['size'][$key],
                                    )
                                );
                                $_FILES = $files;
                                $file = new FilesSimec('anexoaviso', null, 'obras2');
                                $file->setPasta('obras2');
                                $file->setUpload($_POST['arquivo'][$key], null, false);
                                $arqid = $file->getIdArquivo();

                                //salvar tabela anexoaviso
                                if ($arqid) {
                                    $anexoAviso = new Obras2_Modelo_Anexoaviso();
                                    $_POST['arquivo'][] = $arqid;
                                    $anexoAviso->anxavnome = "'{$arquivos['arquivo']['name'][$key]}'";
                                    $anexoAviso->anxavdsc = "'{$_POST['anxavdsc'][$key]}'";
                                    $anexoAviso->anxavtipo = "'{$arquivos['arquivo']['type'][$key]}'";
                                    $anexoAviso->anxavdata = date("Y-m-d");
                                    $anexoAviso->anxavcpf = $_SESSION['usucpf'];
                                    $anexoAviso->anxavstatus = 'A';
                                    $anexoAviso->avsoid = $idAviso;
                                    $anexoAviso->arqid = $arqid;
                                    $anexoAviso->salvar();
                                }
                            }
                        }
                        $aviso->commit();
                        header("location:obras2.php?modulo=principal/editarAviso&acao=A");
                    } else {
                        echo "<script> alert('Erro ao Salvar');</script>";
                    } 
                }
            break;
        }
    }
    //caso usuário clique sobre o arquivo
    if($_GET['download']){
        if(!$anexoAviso){
            $anexoAviso = new Obras2_Modelo_Anexoaviso();
        }
        $file = new FilesSimec(null,null,'obras2');
        $file->getDownloadArquivo( $_GET['download'] );

        die('<script type="text/javascript">
                            window.close();
                      </script>');
    }


    include APPRAIZ . "includes/cabecalho.inc";
    echo '<br />';
    monta_titulo( 'Início - Aviso Atenção', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');
    ?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

    <body topmargin="0" leftmargin="0">
        <form method="post" id="formulario" name="formulario" enctype="multipart/form-data">
            <input type="hidden" name="tramitacao" id="tramitacao" value="" />
            <input type="hidden" name="aviso[avsoid]" value="<?php echo $avsoid; ?>" />
            <input type="hidden" name="excluirAviso" value="" />
            <input type="hidden" name="excluirFile" value="" />

            <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
                <tr>
                    <td class="SubTituloDireita" width="20%">Aviso:</td>
                    <td>
                        <?php echo campo_textarea("aviso[avsodescricao]", 'S', $habilita, '', 200, 10, '', '', '', '', '', '', $avsodescricao );?>
                    </td>
                    <td></td>
                </tr>
                    <tr>
                    <td class="SubTituloDireita">Ordem</td>
                    <td>
                        <?php echo campo_texto( "aviso[avsoordem]", 'S', 'S', '', 11, 10, '#########', '', 'right', '', 0, '', '',$avsoordem); ?>
                    </td>
                </tr>
                </tr>
                    <tr>
                    <td class="SubTituloDireita">Válido Até</td>
                    <td>
                        <?php
                        echo campo_data2('aviso[avsovalidade]', 'S', 'S', '', 'S', '', '', $avsovalidade);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Alerta</td>
                    <td>
                        <!--<input type="checkbox" value="true" name="aviso[avsoalert]" <?php //echo (($avsoalert == 't') ? 'checked="checked"' : ''); ?> />-->
                        <input type="radio" name="aviso[avsoalert]" value="S" <?=$avsoalert == 'S' ? 'checked' : '' ?> <?=$boHabilitado ?>><label>Sim.</label>
                        <input type="radio" name="aviso[avsoalert]" value="N" <?=$avsoalert == 'N' ? 'checked' : '' ?> <?=$boHabilitado ?>><label>Não.</label>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" style="width: 190px;">Anexos</td>
                    <td colspan="2">
                        <div>
                            <table id="table_anexos" align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                                <tr>
                                    <td class="">
                                            <div style=""><img src="/imagens/gif_inclui.gif" alt=""/> <a href="" id="adicionar_anexo">Adicionar</a></div>
                                    </td>
                                    <td>Nome</td>
                                    <td>Descrição</td>
                                </tr>

                                <?php
                                if(!$anexoAviso){
                                    $anexoAviso = new Obras2_Modelo_Anexoaviso();
                                }
                                $arquivos = ($avsoid) ? $anexoAviso->pegaArquivosPorAvsoid($avsoid) : array();
                                if (!empty($arquivos)): ?>
                                    <?php foreach ($arquivos as $arquivo): ?>
                                        <tr class="anexos">
                                            <td class="SubTituloEsquerda" style="width: 56px;">
                                                <div style=""><a class="excluir_anexo_tabela" href="javascript:void(0);" onclick="excluirFile(<?php echo$arquivo['arqid']?>);" ><img src="/imagens/excluir.gif" alt=""/>Excluir</a></div>
                                            </td>
                                            <td class="SubTituloEsquerda">
                                                <input type="hidden" value="<?=$arquivo['arqid']?>" name="arquivo[]" id="arquivo"/>
                                                <a target="_blank" href="/obras2/obras2.php?modulo=principal/editarAviso&acao=A&download=<?=$arquivo['arqid']?>">
                                                    <?=$arquivo['arqnome']?>.<?=$arquivo['arqextensao']?>
                                                </a>
                                            </td>
                                            <td class="SubTituloEsquerda"><input  type="text" maxlength="255" size="50" value="<?=$arquivo['anxavdsc']?>" name="anxavdsc[]" id="anxavdsc"/></td>
                                        </tr>
                                    <?php endforeach; ?>
                                <?php else: ?>
                                    <tr class="anexos">
                                        <td class="SubTituloEsquerda" style="width: 56px;">
                                            <span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span>
                                        </td>
                                        <td class="SubTituloEsquerda"><input type="file" name="arquivo[]" id="arquivo"/></td>
                                        <td class="SubTituloEsquerda"><input maxlength="255" size="50" type="text" name="anxavdsc[]" id="anxavdsc"/></td>
                                    </tr>
                                <?php endif ?>
                            </table>
                        </div>
                    </td>
                </tr>
                <tr align="center">
                    <td></td>
                    <td>
                        <input type="reset"  name="botao" value="Limpar" />
                        <input type="button"  name="botao" value="Novo" onClick="window.location.replace('obras2.php?modulo=principal/editarAviso&acao=A');return false;"/>
                        <input type="button"  name="botao" value="Salvar" onclick="salvaSolicitacao();"/>
                    </td>
                </tr>
            </table>
        </form>
    </body>
    <?php
    $acao = "'<center>
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/alterar.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: alterarAviso(' || avso.avsoid || ');\"
                            title=\"Alterar Aviso\">
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/excluir.gif\"
                            style=\"cursor: pointer; margin-left: 3px;\"
                            onclick=\"javascript: excluirAviso(' || avso.avsoid || ');\"
                            title=\"Excluir Aviso\">
              </center>'";

    $query = "SELECT DISTINCT
                {$acao} AS acao,
                avsodescricao || ' ', 
                avsoordem, 
                CASE WHEN avsoalert = 'S' THEN 'Sim' ELSE 'Não' END as alerta, 
                CASE WHEN arqid IS NOT NULL THEN 'Sim' ELSE 'Não' END as anexo
            FROM
                obras2.avisoobras as avso
            LEFT JOIN
                obras2.anexoaviso as anxa ON anxa.avsoid  = avso.avsoid ORDER BY avsoordem ASC";
                
    $cabecalho = array('&nbsp;&nbsp;&nbsp;&nbsp;Ação&nbsp;&nbsp;&nbsp;&nbsp;','Aviso', 'Ordem', 'Alerta', 'Anexos');
    $db->monta_lista($query,$cabecalho,2000,20,'','','');
    ?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
    <script type="text/javascript">
        jQuery(function(){
            $('#adicionar_anexo').click(function (e) {
                $('#table_anexos').append($('<tr class="anexos anexos-base"><td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span></td><td class="SubTituloEsquerda"><input type="file" name="arquivo[]" id="arquivo"/></td><td class="SubTituloEsquerda"><input maxlength="255" size="50" type="text" name="anxavdsc[]" id="anxavdsc"/></td></tr>').removeClass('anexos-base'));
                e.preventDefault();
            });
            $('.excluir_anexo').live('click',function (e) {
                $(this).parents('tr.anexos').remove();
                e.preventDefault();
            });
        });
        function alterarAviso( avsoid ){
            $(location).attr('href', '?modulo=principal/editarAviso&acao=A&avsoid='+avsoid);
        }    

        function excluirAviso( avsoid){
            $('[name="tramitacao"]').val('excluirAviso');
            $('[name="excluirAviso"]').val(avsoid);
            $('#formulario').submit();
        }

        function excluirFile( arqid ){
            if(confirm("Tem certeza que deseja excluir o arquivo?")){
                $('input[name="excluirFile"]').val(arqid);
                $('input[name="tramitacao"]').val('excluirFile');
                $('#formulario').submit();
            }
        }

        function salvaSolicitacao(){
            var msg = '';
            if(!$("textarea[name='aviso[avsodescricao]']").val()){
                msg += "Informe o Aviso.\n";
                $("textarea[name='aviso[avsodescricao]']").focus();
            }
            if(!$('input[name="aviso[avsoordem]"]').val()){
                msg += "Informe a Ordem que o aviso deverá ser exibido.\n";
                $('input[name="aviso[avsoordem]"]').focus();
            }

            var isChecked = $('input[name="aviso[avsoalert]"]:checked').val();
            if (!isChecked) {
                msg += "Selecione campo Alerta";
            }

            if( (!$('#arquivo').val()) && ($('#anxavdsc').val()) ){
                msg += "Adicione o arquivo antes de informar a descrição.";
                $('#anxavdsc').focus();

            }

            if(msg != ''){
                alert(msg);
                return false;
            }
            $('input[name="tramitacao"]').val('salvar');
            $('#formulario').submit();
        }

        function focusOrdem(){
            $('input[name="aviso[avsoordem]"]').focus();
        }

    </script><?php
}else{
    echo "<script>
                alert('Acesso negado.');
                window.location.href = 'obras2.php?modulo=principal/listaObras&acao=A';
          </script>";
     
}

