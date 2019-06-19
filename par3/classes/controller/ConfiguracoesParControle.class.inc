<?php
class Par3_Controller_ConfiguracoesParControle extends Controle
{
    public function __construct()
    {
        parent::__construct();
    }

    public function salvar($arrPost)
    {
        $mconfiguracoesPar = new Par3_Model_ConfiguracoesPar();
        $url = "par3.php?modulo=sistema/tabelaapoio/geral/configuracoes_par&acao=A";
        try {
            if(empty($arrPost['copvalor']) || $arrPost['copvalor'] < 0){
                throw new \Exception("Informe um número inteiro e positivo para esta configuração.");
            }
            if(empty($arrPost['tcpid'])){
                throw new \Exception("Selecione um tipo de configuração.");
            }

            $sql = $mconfiguracoesPar->updateStatusConfPar($arrPost['tcpid']);
            $mconfiguracoesPar->executar($sql);

            //Salva tipo de configuração
            $arrPost['usucpf'] = $_SESSION['usucpf'];
            $arrPost['copstatus'] = 'A';
            $arrPost['copdtinclusao'] = date('Y-m-d H:m:s');
            $mconfiguracoesPar->popularDadosObjeto($arrPost);

            $novoConf = $mconfiguracoesPar->salvar();

            $mconfiguracoesPar->commit();

            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mconfiguracoesPar->rollback();
            simec_redirecionar($url, 'error', $e->getMessage());
        }

    }//end salvar()

    public function listaConfiguracoesPar($arrPost = null)
    {
        global $disabled;

        $sql = Par3_Model_ConfiguracoesPar::pegarSQLlista($arrPost);

        $cabecalho = array('Tipo', 'Valor', 'Usuário', 'Data Inclusão', 'Status');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas(['id']);
        $listagem->addCallbackDeCampo('copdtinclusao','formata_data_hora');
        $listagem->addCallbackDeCampo('copstatus','formata_status');

        if($disabled == ''){
            //$listagem->addAcao('delete', 'deletar');
        }
        $listagem->turnOnOrdenacao();
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::RELATORIO_PAGINADO);

    }//end listaEquipe()
}