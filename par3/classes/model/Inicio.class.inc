<?php
/**
 * Classe de mapeamento da entidade par3.inicio
 *
 * @category Class
 * @package  A1
 * @author   MARCUS VINÍCIUS LOUVEM ROCHA <marcus.rocha@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-10-2015
 * @link     no link
 */



/**
 * Par3_Model_Inicio
 *
 * @category Class
 * @package  A1
 * @author    <Marcus V L Rocha>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Par3_Model_Inicio extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.inicio';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'iniid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'mnuid' => array('tabela' => 'menu', 'pk' => 'mnuid'),
        'pflcod' => array('tabela' => 'perfil', 'pk' => 'pflcod'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'iniid' => null,
        'mnuid' => null,
        'pflcod' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'iniid' => array(  'Digits'  ),
            'mnuid' => array( 'allowEmpty' => true, 'Digits'  ),
            'pflcod' => array( 'allowEmpty' => true, 'Digits'  ),
        );
    }//end getCamposValidacao($dados)

    public function pegarInicioPorCpf($usucpf)
    {
    	if ( ! function_exists('pegaPerfils')) 
    	{
    		throw new Exception('Erro - A função pegaPerfils() não foi declarada, é necessário acessar a tela inicial do PAR 3 pelo fluxo normal do SIMEC');
    	}
    	
    	$arrPerfil = pegaPerfils($usucpf);
    	$arrPerfilValido = carregarPerfisResp();

    	if ( in_array(PAR3_PERFIL_ANALISTA_PROFE, $arrPerfil) || in_array(PAR3_PERFIL_PROCURADOR_PROFE, $arrPerfil) ) {
            return 'par3.php?modulo=principal/listaObras&acao=A';
        }
        
        if ($this->_verificaPendenciaCAE($arrPerfil)) {
            return 'par3.php?modulo=principal/cae/conselheiro&acao=A';
        }

//        $modelCacs = new Par3_Model_CACS();
//        if ($modelCacs->verificaPendenciaCacs($arrPerfil)) {
//            return 'par3.php?modulo=principal/cacs/conselheiro&acao=A';
//        }

    	$possuiResp = checaPerfilValido( $arrPerfilValido, $arrPerfil);

    	$responsabilidade = new Par3_Model_UsuarioResponsabilidade();

        $arResponsab = $responsabilidade->validarUsuarioResponsabilidade($usucpf);

        //caso seja um usuario com apenas uma unidade vinculada (municipal OU estadual), faz o redirecionamento direto para a unidade
        if ($possuiResp == 'S' && !($_REQUEST['redirect'])) {

        	$respons = $responsabilidade->retornaUsuarioResponsabilidade($usucpf);

        	if ($respons['muncod'] == '5300108') {
        		unset($respons['muncod']);
        		$respons['estuf'] = 'DF';
        	}

       		if (!empty($respons['muncod'])) {

       			$_REQUEST['muncod'] = $respons['muncod'];
       			$_REQUEST['itrid'] = 2;
       			$inuid = $responsabilidade->recuperaInuidUsuarioResponsabilidade($_REQUEST);
       			$location = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid=".$inuid;
       			return $location;

       		} elseif(!empty($respons['estuf'])) {

	        	$_REQUEST['estuf'] = $respons['estuf'];
	        	$_REQUEST['itrid'] = $_REQUEST['estuf'] == 'DF' ? 3 : 1;
	        	$inuid = $responsabilidade->recuperaInuidUsuarioResponsabilidade($_REQUEST);
	        	$location = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid=".$inuid;

        		return $location;
       		} else {
                $location = 'par3.php?modulo=principal/listaMunicipios&acao=A';
                return $location;
            }
        } else {
	        $sql = "SELECT mnulink from seguranca.perfilusuario pru
	        join seguranca.perfil pfl on pru.pflcod = pfl.pflcod
	        join {$this->stNomeTabela} ini on ini.pflcod = pfl.pflcod
	        join seguranca.menu men on men.mnuid = ini.mnuid
	        where usucpf = '{$usucpf}' and pfl.sisid = 231
	        order by pflnivel limit 1 ";

	        return $this->pegaUm($sql);
        }
    }

    /**
     * Verifica preenchimento de dados do CAE, mesmo usuário possuí multiplos perfis
     * @param $arrPerfil
     * @return bool
     */
    protected function _verificaPendenciaCAE($arrPerfil)
    {
        if (!is_array($arrPerfil)) {
            $arrPerfil = [$arrPerfil];
        }

        $flag = false;
        if (in_array(Par3_Model_UsuarioResponsabilidade::CONSELHEIRO, $arrPerfil)) {

            $flag = true;
            $modelCAE = new Par3_Model_CAE();
            if ($objCAE = $modelCAE->carregarConselheiroCPF($_SESSION['usucpf'])) {
                if (empty($objCAE->entdtnascimento) ||
                    empty($objCAE->enttelefonecomercial) ||
                    empty($objCAE->enttelefonecomercial) ||
                    empty($objCAE->entemail) ||
                    empty($objCAE->entsexo) ||
                    empty($objCAE->cacfuncao) ||
                    empty($objCAE->cctid) ||
                    empty($objCAE->entnivelensino) ||
                    empty($objCAE->cacoutroconselho) ||
                    empty($objCAE->cacocupacao) ||
                    empty($objCAE->cacrenda))
                {
                    $flag = true;
                } else {
                    $flag = false;
                }
            }
        }

        return $flag;
    }



}//end Class
?>