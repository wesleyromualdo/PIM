
<?php
require APPRAIZ . 'includes/cabecalho.inc';

if (!empty($_REQUEST['pfaid'])) {
    $proadesao = new Par3_Model_Proadesao($_REQUEST['pfaid']);
    // se existir arqid então retorno o nome do arquivo
    if (!empty($proadesao->arqid)) {
        $nomeArquivo = $proadesao->pegaArquivoResolucao($proadesao->arqid);
    }
    // arquivo do manual
    if (!empty($proadesao->arqidmanual)) {
        $nomeArquivoManual = $proadesao->pegaArquivoResolucao($proadesao->arqidmanual);
    }
} else {
    $proadesao = new Par3_Model_Proadesao();
}
$listaprogramas = $proadesao->pegarAdesoes();

switch ($_REQUEST['req']) {

    case 'salvarDados':
        $file = new FilesSimec("proadesao", array(), "par3");
        // se houver upload de arquivo de resolução
        if (!empty($_FILES['arquivoresolucao'])) {            
//            $file = new FilesSimec("proadesao", array(), "par3");
            $file->setUpload('Resolução do Programa Escola Acessível', false, false);
            $arqid = $file->getIdArquivo();        
            $_POST['arqid'] = $arqid;
        } else {
            $_POST['arqid'] = $_POST['editararquivoresolucao']; 
        }
        
        // se houver upload de arquivo do manual
        if (!empty($_FILES['arquivomanual'])) {            
            $file->setUpload('Manual do Programa Escola Acessível', 'arquivomanual', false);
            $arqidmanual = $file->getIdArquivo();        
            $_POST['arqidmanual'] = $arqidmanual;
        } else {
            $_POST['arqidmanual'] = $_POST['editararquivomanual']; 
        }
        
        $proadesao->popularDadosObjeto($_POST);
        $proadesao->salvar();
        $proadesao->commit();
        $url = "par3.php?modulo=sistema/administracao/cadAdesao&acao=A";
        simec_redirecionar($url, 'success');

        break;

    default:

        break;
}

echo $simec->title('Lista de adesão dos programas', '');
?>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox">
            <form method="post" name="formulario" id="formulario" class="form form-horizontal" enctype="multipart/form-data">
                <input type="hidden" name="pfaid" value="<?php echo $_GET['pfaid'] ?>">
                <input type="hidden" name="req" value="salvarDados">
                <div class="ibox-title">
                    <h3>Dados da Adesão</h3>
                </div>
                <div class="ibox-content">

                    <?php
                    $disabled = false;
                    $radioStatus = array('A' => 'Sim', 'I' => 'Não');
                    $sql = "SELECT
                                        prgid as codigo,
                                        prgdsc as descricao
                                    FROM par3.programa
                                   ";

                    echo $simec->select("prgid", 'Programa', $proadesao->prgid, $sql, array("required"));


                    echo $simec->input('pfaano', 'Ano', $proadesao->pfaano, array('maxlength' => '4'));

                    echo $simec->data('pfadatainicial', 'Data de início', $proadesao->pfadatainicial, array($disabled, 'required'));
                    echo $simec->data('pfadatafinal', 'Data final', $proadesao->pfadatafinal, array($disabled, 'required'));
                    echo $simec->input('pfaicone', 'Nome do ícone', $proadesao->pfaicone, array('maxlength' => '255'));
                    echo $simec->input('pfafundocor', 'Cor de fundo', $proadesao->pfafundocor, array('maxlength' => '255'));
                    echo $simec->input('pfabackground', 'Imagem de fundo', $proadesao->pfabackground, array('maxlength' => '255'));                    
                    // arquivo de resolução
                    echo $simec->input('arquivoresolucao', 'Arquivo da Resolução', null, array('type' => 'file','help' =>'Formatos permitidos: PDF', 'onchange' => 'validarArquivoResolucao(this)'), array('input-size' => '6'));                    
                    if( !empty($proadesao->arqid) ) : ?>
                        <div class="form-group ">
                            <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label"></label>
                            <div class="col-sm-6 col-md-6 col-lg-6 ">
                                <span class="text-danger"><?php echo $nomeArquivo; ?></span>
                            </div>
                        </div> 
                    <?php                    
                    endif;                    
                    // arquivo do manual
                    echo $simec->input('arquivomanual', 'Arquivo do Manual', null, array('type' => 'file','help' =>'Formatos permitidos: PDF', 'onchange' => 'validarArquivoManual(this)'), array('input-size' => '6'));
                    if( !empty($proadesao->arqidmanual) ) : ?>
                        <div class="form-group ">
                            <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label"></label>
                            <div class="col-sm-6 col-md-6 col-lg-6 ">
                                <span class="text-danger"><?php echo $nomeArquivoManual; ?></span>
                            </div>
                        </div> 
                    <?php                    
                    endif;
                    // caso já exista um arqid da resolução
                    echo $simec->input('editararquivoresolucao', '', $proadesao->arqid, array('type' => 'hidden'), array('input-size' => '6'));                    
                    // caso já exista um arqidmanual do manual
                    echo $simec->input('editararquivomanual', '', $proadesao->arqidmanual, array('type' => 'hidden'), array('input-size' => '6'));  
                    
                    echo $simec->radio('pfastatus', 'Status', $proadesao->pfastatus, $radioStatus, array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
                    echo '<div class="form-group ">
                                <label for="pfaicone" class="col-sm-3 col-md-3 col-lg-3 control-label">Variáveis possíveis nos termos de Adesão: </label>
                                    <div class="col-sm-9 col-md-9 col-lg-9 ">
                                        #nome_entidade#<br>
                                        #data_atual#<br>
                                        #dirigente#<br>
                                        #dirigente_cpf#<br>
                                        #escolas_prioritarias#
                                    </div>
                                </div>';
                    echo $simec->textarea('pfatermomunicipio', 'Termo de Adesão do Município', $proadesao->pfatermomunicipio, array('class' => 'summernote'));
                    echo $simec->textarea('pfatermoestado', 'Termo de Adesão do Estado', $proadesao->pfatermoestado, array('class' => 'summernote'));
                    echo $simec->textarea('pfamsgboasvindas', 'Mensagem de Boas vindas da adesão', $proadesao->pfamsgboasvindas, array('class' => 'summernote'));
                    echo $simec->textarea('pfamsgdeaceitacao', 'Mensagem que será exibida depois da aceitação da entidade', $proadesao->pfamsgdeaceitacao, array('class' => 'summernote'));
                    echo $simec->textarea('pfamsgdenaoaceitacao', 'Mensagem que será exebida caso a entidade não aceite o termo', $proadesao->pfamsgdenaoaceitacao, array('class' => 'summernote'));
                    echo $simec->textarea('pfasobre', 'Texto sobre a adesão do programa', $proadesao->pfasobre, array('class' => 'summernote'));
                    ?>
                    <div class="ibox-footer">
                        <div class="row">
                            <div class="col-lg-3"></div>
                            <div class="col-lg-9">
                                <div>
                                    <button type="submit" class="btn btn-success salvar"><i class="fa fa-save"></i> Salvar</button>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </form>
            <div class="ibox">
                <div class="ibox-title">
                    <h3>Lista da Adesão</h3>
                </div>
                <div class="ibox-content">
                    <table class="table table-hover dataTable">
                        <thead>
                            <th class="text-center">Ação</th>
                            <th>Programa</th>
                            <th>Data de início</th>
                            <th>Data de fim</th>
                            <th>Ano</th>
                            <th>Status</th>
                        </thead>
                        <tbody>
                            <?php foreach ($listaprogramas as $key) { ?>
                                <tr>
                                    <td class="text-center">
                                        <a class="btn btn-warning btn-sm" href="?modulo=sistema/administracao/cadAdesao&acao=A&req=edit&pfaid=<?php echo $key['pfaid'] ?>" title="Editar"><i class="fa fa-pencil"></i></a>
                                        <?php if ($key['pfaid'] == 12) : ?>
                                            <a class="btn btn-white btn-sm" href="?modulo=sistema/administracao/importarEscolaPrograma&acao=A&requisicao=carregarDados&pfaid=<?php echo $key['pfaid'] ?>" title="Upload Escolas"><i class="fa fa-upload"></i></a>
                                        <?php endif; ?>
                                    </td>
                                    <td><?php echo $key['prgdsc'] ?></td>
                                    <td><?php echo date("d/m/Y",strtotime($key['pfadatainicial'])) ?></td>
                                    <td><?php echo date("d/m/Y",strtotime($key['pfadatafinal'])) ?></td>
                                    <td><?php echo $key['pfaano'] ?></td>
                                    <td class="text-center">
                                        <?php if ($key['pfastatus'] == 'A') { ?>
                                            <span class="label label-success">Ativo</span>
                                        <?php } else {?>
                                            <span class="label label-danger">Inativo</span>
                                        <?php } ?>
                                    </td>
                                </tr>
                            <?php } ?>
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
    </div>
</div>


<script>
    
    function validarArquivoResolucao(file){
        var extPermitidas = ['pdf'];
        var extArquivo = file.value.split('.').pop();        
        if(typeof extPermitidas.find(function(ext){ return extArquivo == ext; }) == 'undefined') {
            alert('Extensão "' + extArquivo + '" não permitida!, use somente arquivo .PDF');
            $("#arquivoresolucao").val("");
        } else if (file.files[0].size > 10000000){
            alert("O arquivo deve ter menos de 10 MB");
            file.value = null;
            $("#arquivoresolucao").val("");
        }        
    }
    
    function validarArquivoManual(file){
        var extPermitidas = ['pdf'];
        var extArquivo = file.value.split('.').pop();        
        if(typeof extPermitidas.find(function(ext){ return extArquivo == ext; }) == 'undefined') {
            alert('Extensão "' + extArquivo + '" não permitida!, use somente arquivo .PDF');
            $("#arquivomanual").val("");
        } else if (file.files[0].size > 10000000){
            alert("O arquivo deve ter menos de 10 MB");
            file.value = null;
            $("#arquivomanual").val("");
        }        
    }

</script>