<?php
/**
 * $Id: Workflow.php 97554 2015-05-19 21:13:18Z maykelbraz $
 */

/**
 * Ação de exibição da barra do workflow.
 * O nome da função, ao invés de ser utilizado como callback javascript,
 * é utilizado como o parâmetro 'requisicao' e enviado ao servidor, para
 * executar a criação da barra de workflow.
 *
 * Exemplo:
 * <pre>
 * array(2) {
 *   ["requisicao"]=>
 *     string(12) "drawWorkflow"
 *   ["params"]=>
 *     array(2) {
 *       [0]=>
 *         string(5) "70549"
 *       [1]=>
 *         string(8) "30430693"
 *   }
 * }
 * </pre>
 */
class Simec_Listagem_Acao_NotaDemandasFies extends Simec_Listagem_Acao
{
    protected $icone = 'file';
    protected $titulo = 'Anotação';
    protected $cor = 'warning';
    //protected $callbackJS = 'onClickAcaoAnotacao';

    protected function renderAcao()
    {                
        //separando e removendo aspas dos parametros
        $params = $this->getCallbackParams(true);
                                                      
        $dmdid = trim( str_replace("'","", $params['dmdid']) );
        $nota = trim(  str_replace( array("'", '"'), array("", "&quot;"), $params['nota'])  );
        $esdid = trim( str_replace("'","", $params['esdid']) );
        $nome = trim(  str_replace( array("'", '"'), array("", "&quot;"), $params['nome_editor_nota'])  );
        $data = trim(  str_replace( array("'", '"'), array("", "&quot;"), $params['data_edicao_nota'])  );
                               
        //formatando o texto para que apenas a primeira letra de cada parte do nome fique em maiusculo
        $nome = ucwords( strtolower($nome) );
        
        //concatenando o texto exibido na tooltip
        $tooltip = $nota." -- ".$nome;
        
        //verificando se já existe uma nota para a demanda
        if (!empty( trim($nota) )) {
            //se houver, muda a cor e e exibe o tooltip
            $this->cor =  'warning';            
            $tooltip = 'data-toggle="tooltip" title="'.$tooltip.'" data-nota="'.$nota.'" data-nome="'.$nome.'" data-data="'.$data.'" ';
        } else {
            //se não tiver texto usa outra cor sem tooltip
            $this->cor =  'default';            
            $tooltip = '';
        }
        
        // -- Ação avançada
        $acao = 
        '<a class="acao-nota-demandasfies" '.$tooltip.' data-dmdid="'.$dmdid.'" data-esdid="'.$esdid.'" >
            '.$this->renderGlyph().'
        </a>';
       
        return $acao;
    }
    
    /**
     * Sobrescrita do método getCallbackParams() para retornar os parametros adicionais como array associativo.
     * 
     * Também remove as aspas dos parâmetros.
     * {@inheritDoc}
     * @see Simec_Listagem_Acao::getCallbackParams()
     */
    protected function getCallbackParams($paramsComoArray = false)
    {
        $params = array();
                
        // -- parametros extras
        foreach ($this->parametrosExtra as $param) {
            if (!key_exists($param, $this->dados)) {
                trigger_error("O parâmetro '{$param}' não existe no conjunto de dados da listagem.", E_USER_ERROR);
            }
            $params[$param] = "'{$this->dados[$param]}'";
        }
        // -- parametros extras
        foreach ($this->parametrosExternos as $key => $param) {
            $params[$key] = "'{$param}'";
        }
          
        /*  se a flag de parâmetro como array foi setada como true apenas não executa o código a seguir
         *  porque o array já é associativo até o momento
         */
        if (!$paramsComoArray) {
                      
            if (!is_null($parametroinicial)) {
                array_unshift($params, "'{$parametroinicial}'");
            }
            $params = implode(', ', $params);
        }
                
        return $params;
    }
}
