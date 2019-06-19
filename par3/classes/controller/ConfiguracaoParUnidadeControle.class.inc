<?php

class Par3_Controller_ConfiguracaoParUnidadeControle extends Controle
{
    public function __construct()
    {
        parent::__construct();
    }

    public function salvar($arrPost)
    {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrPost['inuid']}&menu=tecnico";
        $mConfiguracaoUnidade = new Par3_Model_ConfiguracaoParUnidade();
//        $modelUnidade = new Par3_Model_InstrumentoUnidadeEntidade();
//        $sql = $modelUnidade->montarSQLSimplesEntidades($arrPost);
//        $integrantes =  $modelUnidade->carregar($sql);
//        $modelUnidade->commit();
//
//        //ver(count($integrantes),d);
        try {

//            if (empty($arrPost['couvalor']) || $arrPost['couvalor'] < 0) {
//                throw new \Exception("Informe um limite válido para a configuração.");
//            }

            $sql = $mConfiguracaoUnidade->updateStatusConfUniPar($arrPost['tcpid'], $arrPost['inuid']);
            $mConfiguracaoUnidade->executar($sql);

            //Salva tipo de configuração
            //Dados da sessão: usucpf
            //Dados do request inuid,tcpid,couvalor
            $arrPost['usucpf'] = $_SESSION['usucpf'];
            $arrPost['coustatus'] = "A";
            $arrPost['coudtinclusao'] = date('Y-m-d H:m:s');

            $mConfiguracaoUnidade->popularDadosObjeto($arrPost);

            $mConfiguracaoUnidade->salvar();

            $mConfiguracaoUnidade->commit();

            simec_redirecionar($url, 'success','O limite de integrantes foi alterado com sucesso.');
        } catch (Exception $e) {
            $mConfiguracaoUnidade->rollback();
            simec_redirecionar($url, 'error', $e->getMessage());
        }
    }

    public function retornaConfiguracaoUnidadeLimite($inuid, $tcpid)
    {
        $mConfiguracaoUnidade = new Par3_Model_ConfiguracaoParUnidade();
        $sql = $mConfiguracaoUnidade->retornaConfiguracaoUnidadeSQL($inuid, $tcpid);
        $objConfiguracaoUnidade = $mConfiguracaoUnidade->pegaUm($sql, 0, 30);
        return $objConfiguracaoUnidade;
    }
}