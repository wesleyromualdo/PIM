<?php

/**
 * Classe de controle da equipe local
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_EquipeLocal
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-10-2015
 * @link     no link
 */
class Par3_Controller_EquipeLocal extends Controle
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
    public function formEquipeLocal($disabled = 'disabled', $objPessoaEquipe = null)
    {
        global $simec;

		$sqlSeguimento = Par3_Model_EquipeLocalSeguimento::pegarSQLSelectCombo();
		$sqlCargo = Par3_Model_EquipeLocalCargo::pegarSQLSelectCombo();

		echo '<input type="hidden" name="entid" value="'.$objPessoaEquipe->entid.'"/>';
		echo '<input type="hidden" name="entcpf_old" value="'.formatar_cpf($objPessoaFisica->entcpf).'"/>';
		echo $simec->cpf('entcpf', 'CPF', $objPessoaFisica->entcpf, array('class' => 'cpf', $disabled, 'data-pessoa' => true, 'data-pessoa-campos' => '{"#entnome": "no_pessoa_rf"}', 'required'));
        echo $simec->input('entnome', 'Nome', $objPessoaEquipe->entnome, array('maxlength' => '255', $disabled, 'required', 'readonly'));
        echo $simec->select('eseid', 'Segmento', $objPessoaEquipe->eseid, $sqlSeguimento, array($disabled, 'required'));
    	echo $simec->input('elosegmentooutros', 'Qual o segmento?', $objPessoaEquipe->elosegmentooutros, array('maxlength' => '255', $disabled), array('visible' => false));
        echo $simec->select('ecaid', 'Cargo/Função', $objPessoaEquipe->ecaid, $sqlCargo, array($disabled, 'required'));
        echo $simec->input('entcargo', 'Cargo/Função', $objPessoaFisica->entcargo, array('maxlength' => '255', $disabled, 'required'), array('visible' => false));
    	echo $simec->input('elocargooutros', 'Qual o cargo?', $objPessoaEquipe->elocargooutros, array('maxlength' => '255', $disabled), array('visible' => false));
        echo $simec->email('entemail', 'E-mail', $objPessoaEquipe->entemail, array('class' => 'email', $disabled, 'required'));

        echo "<script>
                $('#eseid').change(function(e) {
                   	e.preventDefault();
                   	var options = $('#ecaid');
        			var other = $('#entcargo');
                   	$.ajax({
                		url: window.location.href,
                		data: {'req':'carregarCargos', 'eseid':$(this).val()},
                		method: 'post',
                		success: function (result) {
                			var result = JSON.parse(result);

	        				if (result) {
        						other.val('');
	        					other.removeAttr('required').closest('.form-group').addClass('hidden');

        						options.empty();
	        					options.attr('required', 'required').closest('.form-group').removeClass('hidden');

		               			$.each(result, function() {
	    	           			    options.append(new Option(this.descricao, this.codigo));
	        	       			});

	                			options.focus();
	               	            options.trigger('chosen:updated');
	        				} else {
        						options.empty();
	        					options.removeAttr('required').closest('.form-group').addClass('hidden');
        						options.trigger('chosen:updated');

        						other.attr('required', 'required').closest('.form-group').removeClass('hidden');
        					}
	               		}
                	});
            		if ($(this).val() == '12') {
    					$('#elosegmentooutros').closest('.form-group').removeClass('hidden');
            			$('#elocargooutros').closest('.form-group').removeClass('hidden');
    				} else {
    					$('#elosegmentooutros').closest('.form-group').addClass('hidden');
    					$('#elosegmentooutros').val('');
    				}
                });

        		$('#ecaid').change(function(e) {
                    var id = parseFloat($(this).val());
                    var arr = [ 12, 15, 16, 17, 18, 19, 20, 21, 22 ];

        			if(jQuery.inArray( id, arr ) > 0) {
						$('#elocargooutros').closest('.form-group').removeClass('hidden');
					} else {
						$('#elocargooutros').closest('.form-group').addClass('hidden');
						$('#elocargooutros').val('');
					}
                });

              </script>";
    }//end formEquipeLocal()


    /**
     * Função formNovoEquipeLocal
     * - monta o formulario de equipe local
     *
     * @return string escreve a lista.
     *
     */
    public function formNovoEquipeLocal($arrPost)
    {
    	global $simec;

    	$radioSexo = array('f' => 'Feminino', 'm' => 'Masculino');

    	echo '<input type="hidden" name="inuid" value="'.$arrPost['inuid'].'"/>';
    	echo '<input type="hidden" name="tenid" value="'.Par3_Model_InstrumentoUnidadeEntidade::EQUIPE.'"/>';
    	echo $this->formEquipeLocal($disabled, $arrPost);
    }//end formNovoEquipeLocal()


    /**
     * Função listaEquipe
     * - monta a lista de equipe.
     *
     * @return escreve a lista.
     *
     */
    public function listaEquipe(array $arrPost = [])
    {
        global $disabled;

        $sql = Par3_Model_EquipeLocal::pegarSQLLista($arrPost);

        $cabecalho = array('CPF', 'Nome', 'Função/Cargo', 'Segmento', 'E-mail');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas(['id']);
        $listagem->addCallbackDeCampo('entcpf','formatar_cpf'); // função php para formatar cpf
        if($disabled == ''){
            $listagem->addAcao('delete', 'inativaIntegranteEquipe');
        }
        $listagem->turnOnOrdenacao();
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    }//end listaEquipe()


    /**
     * Função salvar
     * - salvar novo membro da equipe.
     *
     * @return void.
     *
     */
    public function salvar($arrPost)
    {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $modelEquipe             = new Par3_Model_EquipeLocal();

        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrPost['inuid']}&menu=equipe";

        try {
            $arrPost['entcpf'] = str_replace(array('.', '-'), '', $arrPost['entcpf']);

            if ($this->validarRepresentanteExistePorCPF($arrPost['entcpf'], $arrPost['inuid'])) {
            	throw new \Exception("Este CPF já esta cadastrado como representante desta equipe local.");
            }

            if ($this->validarRepresentanteExistePorEmail($arrPost['entemail'], $arrPost['inuid'])) {
            	throw new \Exception("Este E-mail já esta cadastrado como representante desta equipe local.");
            }

            if(!$arrPost['entid']){
                $arrPost['entusucpfinclusao'] = $_SESSION['usucpf'];
            }

            $modelInstrumentoUnidade->popularDadosObjeto($arrPost);

            $arrPost['entid'] = $modelInstrumentoUnidade->salvar();

            $modelEquipe->popularDadosObjeto($arrPost);
            $modelEquipe->salvar();
            $modelEquipe->commit();

            simec_redirecionar($url, 'success');
        } catch(Exception $e) {
            $modelEquipe->rollback();
            simec_redirecionar($url, 'error', $e->getMessage());
        }

    }//end salvar()


    /**
     * Função invativar
     * - inativa membro da equipe.
     *
     * @return void.
     *
     */
    public function invativar($arrPost)
    {
        $modelEquipe = new Par3_Model_EquipeLocal($arrPost['eloid']);
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidadeEntidade($modelEquipe->entid);

        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrPost['inuid']}&menu=equipe";

        try {
            $modelEquipe->elostatus     = 'I';
            $modelEquipe->elodtexclusao = date('Y-m-d G:i:s');
            $modelEquipe->salvar();

            $modelEquipe->commit();

            $modelInstrumentoUnidade->entstatus = 'I';
            $modelInstrumentoUnidade->entdtinativacao = date('Y-m-d H:i:s');
            $modelInstrumentoUnidade->entusucpfinativacao = $_SESSION['usucpf'];
            $modelInstrumentoUnidade->salvar();

            $modelInstrumentoUnidade->commit();

            simec_redirecionar($url, 'success');
            die();
        } catch(Exception $e) {
            $modelEquipe->rollback();
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
    public function validarRepresentanteExistePorCPF($usucpf, $inuid) {
    	$modelEquipe = new Par3_Model_EquipeLocal();

    	$resultado = $modelEquipe->carregaIntegrante($usucpf, $inuid);

    	return $resultado[0]['id'] != '' ? true : false;
    }


    /**
     * Validar se o integrante já faz parte da equipe local
     * - Verificar integrante existente.
     *
     * @return boolean.
     *
     */
    public function validarRepresentanteExistePorEmail($entemail, $inuid) {
    	$modelEquipe = new Par3_Model_EquipeLocal();

    	$resultado = $modelEquipe->carregaIntegrantePorEmail($entemail, $inuid);

    	return $resultado[0]['id'] != '' ? true : false;
    }

}//end class

?>