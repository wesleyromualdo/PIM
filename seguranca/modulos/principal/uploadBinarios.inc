<?php
include APPRAIZ ."includes/cabecalho.inc";
require_once APPRAIZ.'includes/classes/fileSimec.class.inc';
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';
require_once APPRAIZ . 'includes/library/simec/view/Form.php';
$fm = new Simec_Helper_FlashMessage('seguranca/manutencaoBinarios');

if ($_POST) {
    try {
        // verifica se está rodando em produção
        //if (!IS_PRODUCAO) {
        //    throw new Exception('É necessário estar no ambiente de produção');
        //}
        // verifica se o esquema foi informado
        if (empty($_POST['esquema'])) {
            throw new Exception('É necessário informar o esquema do banco de dados');
        }
        // verifica se o arquivo está vazio
        if ($_FILES['arqbinario']['error']) {

           $upError = [
               1 => 'O arquivo carregado excede o tamanho maximo definido no servidor',
               2 => 'O arquivo carregador excede o tamanho definido no formulário',
               3 => 'O arquivo foi carregado parcialmente',
               4 => 'Nenhum arquivo foi carregado',
               6 => 'Está faltando uma pasta temporária',
               7 => 'Houve falha ao escrever o arquivo no disco',
               8 => 'Uma extensão php interrompeu o upload do arquivo'
            ];
           throw new Exception($upError[$_FILES['arqbinario']['error']]);
       }

       $fileSimec = new FilesSimec(null, null, $_POST['esquema']);
       $fileSimec->setUpload($_FILES['arqbinario'], false, false);
       $_POST['arqid'] = $fileSimec->getIdArquivo();

       $resultado = [
           'msg' => "Arquivo salvo com sucesso!",
           'sucesso' => true
        ];
    } catch (Exception $e) {
        $resultado = [
            'msg' => $e->getMessage(),
            'sucesso' => false
        ];
    }

    $fm->addMensagem(
        $resultado['msg'],
        $resultado['sucesso']
        ?Simec_Helper_FlashMessage::SUCESSO
        :Simec_Helper_FlashMessage::ERRO
    );
}

/**
 * Função responsável por recuperar os esquemas do banco de dados
 * @throw Mensagem de erro
 * @return array Equemas do banco de dados
 */
function getSchemas()
{
    $db = new cls_banco();
    return $db->carregar(
        "SELECT DISTINCT schemaname AS schema
         FROM pg_catalog.pg_tables
         WHERE schemaname NOT IN ('pg_catalog', 'information_schema', 'pg_toast')
         ORDER BY schemaname"
     );
}

require(dirname(__FILE__) . "/formulario/formUploadBinarios.inc");
