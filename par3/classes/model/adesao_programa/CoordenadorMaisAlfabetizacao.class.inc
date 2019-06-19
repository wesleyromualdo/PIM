<?php

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

class Par3_Model_CoordenadorMaisAlfabetizacao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.coordenador_mais_alfabetizacao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'cmaid'
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'inuid' => null,
        'adpid' => null,
        'cmatelefone' => null,
        'cmaemail' => null,
        'cmanome' => null,
        'cmacpf' => null,
        'cmaid' => null,
        'cmastatus' => null,
    );


    function Par3_Model_MaisAlfabetizacao()
    {

    }

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCmaposValidacao($dados = array())
    {
        return array(
            'inuid' => array('allowEmpty' => true, 'Digits'),
            'cmatelefone' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 12))),
            'cmaemail' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 255))),
            'cmanome' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 255))),
            'cmacpf' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))),
            'cmastatus' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
            'cmaid' => array('Digits'),
        );
    }

    /**
     * Valida o email do coordenador
     *
     * @param $email
     * @return boolean
     */
    public function validaEmailCoordenador($email)
    {
        $er = "/^(([0-9a-zA-Z]+[-._+&])*[0-9a-zA-Z]+@([-0-9a-zA-Z]+[.])+[a-zA-Z]{2,6}){0,1}$/";

        if (preg_match($er, $email)) {
            return true;
        } else {
            return false;
        }
    }

    public function deletaGeralPorUnidade($adpid)
    {
        if ($adpid) {
            $this->executar("DELETE FROM par3.coordenador_mais_alfabetizacao WHERE adpid = {$adpid}");
            $this->commit();
        }
    }

    /**
     * Valida o email do coordenador
     *
     * @param $email
     * @return boolean
     */
    public function retornaListagemCoordenador($adpid)
    {
        $sqlDados = "
    			SELECT 
					substr(cmacpf, 1, 3) || '.' ||
			        substr(cmacpf, 4, 3) || '.' ||
			        substr(cmacpf, 7, 3) || '-' ||
                    substr(cmacpf, 10) as cpf ,
					cmanome as nome,
					cmaemail as email,
					'(' || substr(cmatelefone, 1, 2) || ')' || substr(cmatelefone, 3, 11)  as telefone
				FROM 
					par3.coordenador_mais_alfabetizacao
				WHERE
					adpid = {$adpid} AND 
    				cmastatus = 'A'";

        $cabecalho = array('CPF', 'Nome', 'E-mail', 'Telefone');
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sqlDados);
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        return;
    }

    public function existeCoordenador($inuid, $adpid)
    {
        if ($adpid) {
            $sql = "SELECT 
        				cmaid
        			FROM 
        				par3.coordenador_mais_alfabetizacao
        			WHERE
        				inuid = {$inuid} AND
                        adpid = $adpid AND
        				cmacpf IS NOT NULL AND
        				cmanome IS NOT NULL AND
        				cmaemail IS NOT NULL AND
        				cmatelefone IS NOT NULL AND
        				cmastatus = 'A'";

            $result = $this->pegaUm($sql);
        }
        return ($result != '') ? true : false;
    }

    public function recuperarCoordenadorPorInuid($inuid, $adpid)
    {
        if ($adpid) {
            $sql = "SELECT 
        				cmaid,
        				cmacpf,
        				cmanome,
        				cmaemail,
        				cmatelefone,
        				cmastatus,
        				inuid
        			FROM 
        				par3.coordenador_mais_alfabetizacao
        			WHERE
        				inuid = {$inuid} AND
                        adpid = $adpid AND
        				cmastatus = 'A'";

            return $this->pegaLinha($sql);
        }
    }

    public function inativaCoordenador($cmaid = null)
    {
        if ($cmaid) {
            $sql = "   		
				UPDATE par3.coordenador_mais_alfabetizacao SET cmastatus = 'I' WHERE cmaid = {$cmaid}
	    	";

            return $this->executar($sql);
        } else {
            return false;
        }
    }

    public function antesSalvar()
    {
        return parent::antesSalvar();
    }

}
