<?php
/**
 * Carga de planilha com dados financeiros.
 *
 * Recebe um arquivo do tipo csv e insere na tabela de carga e, depois, executa
 * o processamento dos dados carregados.
 */
$fm = new Simec_Helper_FlashMessage('spoemendas/carga/nl');

if (!empty($_POST)) {
    if ('importar' == $_POST['requisicao'] && !empty($_FILES)) {
        if (is_uploaded_file($_FILES['carganl_arquivo']['tmp_name']) && UPLOAD_ERR_OK == $_FILES['carganl_arquivo']['error']) {
            $dadosArquivo = file($_FILES['carganl_arquivo']['tmp_name']);
            array_shift($dadosArquivo);
            /* Limpa a tabela de carga  */
            global $db;
            $limpar = "delete from spoemendas.cargavalores;";
            $db->executar($limpar);
            $db->commit();
            foreach (is_array($dadosArquivo) ? $dadosArquivo : [] as $linha) {
                $dados = [];
                list(
                        $dados['emeano'],
                        $dados['emecod'],
                        $dados['nlcod'],
                        $dados['nldata'],
                        $dados['nlvalor'],
                        ) = explode(';', trim($linha));
                $carga = new Spoemendas_Model_Cargavalores();
                $carga->popularDadosObjeto($dados);
                $carga->salvar();
            }
            $carga->commit();

            $fm->addMensagem('Registros carregados com sucesso.');
            header("Location: /spoemendas/spoemendas.php?modulo=principal/cargas/nl/processamento&acao=A");
            die();
        }

        // -- Não foi possível processar o arquivo.
        switch ($_FILES['carganl_arquivo']['error']) {
            case UPLOAD_ERR_OK:
                $msg = 'Arquivo carregado com sucesso.';
                break;
            case UPLOAD_ERR_INI_SIZE:
                $msg = 'O tamanho do arquivo é maior que o permitido.';
                break;
            case UPLOAD_ERR_PARTIAL:
                $msg = 'Ocorreu um problema durante a transferência do arquivo.';
                break;
            case UPLOAD_ERR_NO_FILE:
                $msg = 'O arquivo enviado estava vazio.';
                break;
            case UPLOAD_ERR_NO_TMP_DIR:
                $msg = 'O servidor não pode processar o arquivo.';
                break;
            case UPLOAD_ERR_CANT_WRITE:
                $msg = 'O servidor não pode processar o arquivo.';
                break;
            case UPLOAD_ERR_EXTENSION:
                $msg = 'O arquivo recebido não é um arquivo válido.';
                break;
            default:
                $msg = 'O arquivo enviado é inválido.';
        }

        $fm->addMensagem($msg, Simec_Helper_FlashMessage::ERRO);
        header("Location: {$_SERVER['REQUEST_URI']}");
        die();
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
(new Simec_View_Breadcrumb())
        ->add('Rotinas de carga')
        ->add('NL')
        ->render();

echo $fm;
?>
<script type="text/javascript" src="../library/bootstrap-3.0.0/js/bootstrap.file-input.js"></script>
<script type="text/javascript">
    $(function () {
        $('input[type=file]').bootstrapFileInput();
    });

    function onCarganlSubmit()
    {
        if (!$('#carganl_arquivo').val()) {
            alert('O campo "arquivo" é obrigatório e não pode ser deixado em branco.');
            return false;
        }
        return true;
    }
</script>
<div class="col-lg-12">
    <?php
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec();
    $arquivo = $file->getArquivo('19883091');

    $form = new Simec_View_Form('carganl');
    $form
            ->addFile('Arquivo', 'arquivo', ['modelo' => $arquivo])
            ->addBotoes(['limpar', 'importar'])
            ->setRequisicao('importar')
            ->render();
    ?>
</div>