<?php

/**
 * Classe de controle da equipe local
 *
 * @category Class/Render
 * @package  A1
 * @author   Daniel Fiuza <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 14/02/2017
 * @link     no link
 */
class Par3_Controller_TipoConfiguracoesParControle extends Controle
{
    public function __construct()
    {
        parent::__construct();
    }


    /**
     * Função formEquipeLocal
     * - monta o formulario de equipe local
     *
     * @return string escreve a lista.
     *
     */
    public function formTipoConfiguracao($disabled = 'disabled', $objTipoConfiguracao = null)
    {
        global $simec;

        //$sqlSeguimento = Par3_Model_EquipeLocalSeguimento::pegarSQLSelectCombo();
        $sqlCargo = Par3_Model_EquipeLocalCargo::pegarSQLSelectCombo();

        $mInstrumento = new Par3_Model_Instrumento();
        $instrumentos = array();
        foreach ($mInstrumento->recuperarTodos() as $rsInstrumentos) {
            $instrumentos[$rsInstrumentos['itrid']] = $rsInstrumentos['itrdsc'];
        }

        echo '<input type="hidden" name="tcpid" value="' . $objTipoConfiguracao->tcpid . '"/>';
        echo '<input type="hidden" name="entcpf_old" value="' . formatar_cpf($objTipoConfiguracao->entcpf) . '"/>';
        echo $simec->input('tcpdsc', 'Descrição', $objTipoConfiguracao->tcpdsc, array('maxlength' => '255', $disabled, 'required'));
        echo $simec->checkbox('itr[]', 'Marque as Opções', $value = null, $instrumentos, $attribs = array('required'), $config = array('style' => 'icheckbox_square-green'));
    }//end formTipoConfiguracao()

    /**
     * Função formNovoEquipeLocal
     * - monta o formulario de equipe local
     *
     * @return string escreve a lista.
     *
     */
    public function formNovoTipoConfiguracao($arrPost)
    {
        global $simec;
        echo $this->formTipoConfiguracao($disabled, $arrPost);
    }

    /**
     * Função salvar
     * - salvar novo membro da equipe.
     *
     * @return void.
     *
     */
    public function salvar($arrPost)
    {
        $mTipoConfiguracoesPar = new Par3_Model_TipoConfiguracoesPar();
        $url = "par3.php?modulo=sistema/tabelaapoio/geral/configuracoes_par&acao=A";

        try {

            if(empty($arrPost['tcpdsc'])){
                throw new \Exception("Informe uma descrição ao tipo de configuração!");
            }
            if(empty($arrPost['itr'])){
                throw new \Exception("Selecione pelo menos uma opção de unidade!");
            }

            //Salva tipo de condiguração
            $mTipoConfiguracoesPar->popularDadosObjeto($arrPost);
            $idNovoTipoConfPar = $mTipoConfiguracoesPar->salvar();

            //Salva o relacionamento do tipo de configuração com
            foreach($arrPost['itr'] as $itr){
                $sql = $mTipoConfiguracoesPar->salvarTipoConfiguracao_instrumentoSQL($itr,$idNovoTipoConfPar);
                $mTipoConfiguracoesPar->executar($sql);
            }
            $mTipoConfiguracoesPar->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $mTipoConfiguracoesPar->rollback();

            simec_redirecionar($url, 'error', $e->getMessage());
        }

    }//end salvar()

    public function listarTipoConfiguracoesPar($arrPost = array())
    {
        $mTipoConfiguracoesPar = new Par3_Model_TipoConfiguracoesPar();
        return $mTipoConfiguracoesPar->pegarSQLSelectCombo($arrPost);
    }
    /**
     * Função invativar
     * - inativa membro da equipe.
     *
     * @return void.
     *        $arrPost['eloid']
     */
    public function invativar($arrPost)
    {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidadeEntidade($arrPost['entid']);

        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrPost['inuid']}&menu=tecnico";
        try {
            $modelInstrumentoUnidade->inativaEntidadesInuidPorTipo($arrPost['inuid'], $arrPost['tenid']);
            $modelInstrumentoUnidade->commit();
            simec_redirecionar($url, 'success');
            die();
        } catch (Exception $e) {
            $modelInstrumentoUnidade->rollback();
            simec_redirecionar($url, 'error');
            die();
        }

    }//end salvar()

    /**
     * Validar se o integrante já faz parte da equipe local
     * - Verificar integrante existente.
     *
     * @return boolean.
     *
     */
    public function validarRepresentanteExistePorCPF($usucpf, $inuid, $tenid)
    {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidadeEntidade();

        $resultado = $modelInstrumentoUnidade->recuperarEntidadeUnidadePorCPF($inuid, $usucpf, $tenid);

        return $resultado != '' ? true : false;
    }


    /**
     * Validar se o integrante já faz parte da equipe local
     * - Verificar integrante existente.
     *
     * @return boolean.
     *
     */
    public function validarRepresentanteExistePorEmail($entemail, $inuid, $entid)
    {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidadeEntidade();

        $resultado = $modelInstrumentoUnidade->recuperarEntidadeUnidadePorEmail($entemail, $inuid, $entid);

        return $resultado[0]['id'] != '' ? true : false;
    }

}//end class

?>