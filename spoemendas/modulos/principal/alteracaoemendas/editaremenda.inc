<?php
/**
 * Interface de edição dos dados da emenda.
 *
 * @version $Id: editaremenda.inc 120659 2017-03-22 23:46:39Z maykelbraz $
 */

$fm = new Simec_Helper_FlashMessage('editaremenda');
require_once dirname(__FILE__) . '/../emendas/editaremenda/iniciativas-request.inc';
if (isset($_REQUEST['requisicao'])) {
    switch ($_REQUEST['requisicao']) {
        case 'salvardetalheemenda':

            $novoRegistro = false;
            $dados = $_REQUEST['detalheemenda'];
            if (!empty($dados['emdid'])) {
                $detalheEmenda = new Spoemendas_Model_EmendaDetalhe($dados['emdid']);
            } else {
                $detalheEmenda = new Spoemendas_Model_EmendaDetalhe();
                $dados['emeid'] = $_REQUEST['emeid'];
                $dados['emdtipo'] = 'E';

                $novoRegistro = true;
            }
            $detalheEmenda->popularDadosObjeto($dados);

            if ($emdid = $detalheEmenda->salvar()) {
                $detalheEmenda->commit();

                if ($novoRegistro) {
                    $emendaDetEntidade = new Spoemendas_Model_Emendadetalheentidade();
//                    $emendaDetEntidade->emdid = $emdid;
                }

                $fm->addMensagem('Detalhes da emenda alterados com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível alterar os detalhes da emenda.', Simec_Helper_FlashMessage::ERRO);
            }
            break;
        case 'carregarDetalheEmenda':
            echo simec_json_encode((new Spoemendas_Model_EmendaDetalhe($_REQUEST['dados']['emdid']))->getDados());
            die();
            break;
        case 'apagarBeneficiario':
        case 'pesquisar_cnpj':
        case 'nova_entidade_beneficiada':
            require_once dirname(__FILE__) . '/../emendas/editaremenda/beneficiarios-request.inc';
            die();
            break;
        case 'alterarpartido':
            $dados = $_REQUEST['geral'];

            $autor = new Spoemendas_Model_Autor($dados['autid']);
            $autor->parid = $dados['parid'];
            if ($autor->salvar()) {
                $autor->commit();
                $fm->addMensagem('Partido do autor alterado com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível alterar o partido do autor.', Simec_Helper_FlashMessage::ERRO);
            }
            break;
        default:
            ver($_REQUEST, d);
    }
    header("Location: {$_SERVER['REQUEST_URI']}");
    die();
}

/**
 * Cabeçalho do SIMEC.
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<div class="col-md-12">
<?php
(new Simec_View_Breadcrumb())
    ->add('Alteração de emendas', 'spoemendas.php?modulo=principal/alteracaoemendas/listaremendas&acao=A')
    ->add('Detalhe da emenda')
    ->render();

echo $fm;

$abas = new Simec_Abas(
    "spoemendas.php?modulo=principal/alteracaoemendas/editaremenda&acao=A&emeid={$_REQUEST['emeid']}"
);
$caminhoBaseAbas = dirname(__FILE__) . '/editaremenda/';
$beneficiarioVisaoAdm = true;
$abas->adicionarAba('geral', 'Dados gerais', "{$caminhoBaseAbas}geral.inc")
    ->adicionarAba('detalhes', 'Detalhes da emenda', "{$caminhoBaseAbas}detalhes.inc")
    ->adicionarAba('iniciativas', 'Iniciativas', "{$caminhoBaseAbas}iniciativas.inc")
    ->adicionarAba('beneficiarios', 'Beneficiários', "{$caminhoBaseAbas}beneficiarios.inc")
    ->definirAbaDefault('geral')
    ->render();
?>
</div>