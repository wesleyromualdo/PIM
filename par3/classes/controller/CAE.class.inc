<?php

/**
 * Classe de controle CAE
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */

/**
 * Par_Controle_CAE
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardRoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */
class Par3_Controller_CAE extends Par3_Controller_Entidade
{
    public function __construct()
    {
        parent::__construct();
    }

    /**
     * Função formDadosCAE
     * - monta o formulario de dados de pessoa fisica
     *
     * @return string escreve a lista.
     *
     */
    public function formDadosCAE($disabled = 'disabled', $objCAE = null, $form = null)
    {
        global $simec;

        $radioNomeacao = array('P' => 'Portaria', 'D' => 'Decreto', 'O' => 'Outros');

        echo '<input type="hidden" name="entid" value="'.$objCAE->entid.'"/>';
        echo '<input type="hidden" name="caeid" value="'.$objCAE->caeid.'"/>';
        echo '<input type="hidden" name="endid" value="'.$objCAE->endid.'"/>';
        echo '<input type="hidden" name="tenid" value="'.Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_CAE.'"/>';
        echo '<input type="hidden" id="form" value="'.$form.'"/>';

        Par3_Controller_Entidade::formEnderecoEntidade($disabled, $objCAE, array('mundsc' => $objCAE->endmunicipio));

        echo $simec->telefone('enttelefonecomercial', 'Telefone CAE', $objCAE->enttelefonecomercial, array('class' => 'telefone', $disabled, 'required'));
        echo $simec->email('entemail', 'E-mail', $objCAE->entemail, array('class' => 'email', $disabled, 'required'));
        echo $simec->radio( 'caenomeacao', 'Tipo de Nomeação', $objCAE->caenomeacao, $radioNomeacao, array($disabled), array( 'type' => 'radio radio-info radio', 'style' => 'inline'));
        echo $simec->input('caenomeacaooutro', 'Qual tipo?', $objCAE->caenomeacaooutro, array('maxlength' => '255', $disabled), array('visible' => $objCAE->caenomeacao == "O"));
        echo $simec->input('caedocumento', 'Nº Documento', $objCAE->caedocumento, array('maxlength' => '255', $disabled, 'required'));

        if (!empty($objCAE->caedtinicio) && date('Y-m-d') < $objCAE->caedttermino) {
//         	echo $simec->input('caedtinicio', 'Data de Início', formata_data($objCAE->caedtinicio), array('maxlength' => '14', 'required'), array('icon' => 'fa fa-calendar'));
        	echo $simec->data('caedtinicio', 'Data de Início', $objCAE->caedtinicio, array('maxlength' => '14', 'required'));
        } else {
        	echo $simec->data('caedtinicio', 'Data de Início', $objCAE->caedtinicio, array('maxlength' => '14', $disabled, 'required'));
        }

        echo $simec->input('caedttermino', 'Data de Termino', formata_data($objCAE->caedttermino), array('maxlength' => '14', $disabled, 'required', 'readonly' => true), array('icon' => 'fa fa-calendar'));

        if (!empty($objCAE->arqid)) {
        	$link.= "<a style='padding: 0px;' class='btn' href='par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$_REQUEST['inuid']}&menu=cae&req=downloadArquivo&arqid={$objCAE->arqid}' target='blank'><small style='font-size: 12px;'><i class='fa fa-download'></i> Abrir Anexo</small></a> | ";
        	$link.= "<a style='padding: 0px;' class='btn' href='par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$_REQUEST['inuid']}&menu=cae&req=excluirArquivo&arqid={$objCAE->arqid}&tparquivo=cae&caeid={$objCAE->caeid}'><small style='font-size: 12px;'> <i class='fa fa-trash'></i> Excluir</small></a>";
            echo '<div class="form-group">
                <label for="arqid" class="col-sm-3 col-md-3 col-lg-3 control-label">Anexo I: *</label>
                <div class="col-sm-9 col-md-9 col-lg-9"><div class="input-group" style="margin-top: 4px">'.$link.'</div></div>
            </div>';
        } else {
        	echo $simec->input('arqid', 'Anexo I', null, array('type'=>'file','required', 'help' => 'Portaria ou Decreto de Nomeação do CAE'));
        }
    }//end formDadosCAE()

    /**
     * Função formDadosCAE
     * - monta o formulario de dados de pessoa fisica
     *
     * @return string escreve a lista.
     *
     */
    public function formDadosPresidenteCAE($disabled = 'disabled', $objCAE = null, $form = null)
    {
    	global $simec;

    	echo '<input type="hidden" name="entid" value="'.$objCAE->entid.'"/>';
    	echo '<input type="hidden" name="tenid" value="'.Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_PRESIDENTE_CAE.'"/>';
        echo '<input type="hidden" id="form" value="'.$form.'"/>';
        echo '<input type="hidden" name="cacfuncao" id="cacfuncao" value="P"/>';
        echo '<input type="hidden" name="cactipoocupacao" id="cactipoocupacao" value="T"/>';
    	echo '<input type="hidden" name="entcpf_old" value="'.formatar_cpf($objCAE->entcpf).'"/>';

    	echo $simec->cpf('entcpf', 'CPF do Presidente', $objCAE->entcpf, array('class' => 'cpf', $disabled, 'data-pessoa' => true, 'data-pessoa-campos' => '{"#entnome": "no_pessoa_rf"}'));
    	echo $simec->input('entnome', 'Nome do Presidente', $objCAE->entnome, array('maxlength' => '255', $disabled, 'required', 'readonly' => 'readonly'));
    	echo $simec->telefone('enttelefonecomercial', 'Telefone Principal', $objCAE->enttelefonecomercial, array('class' => 'telefone', $disabled, 'required'));
    	echo $simec->telefone('enttelefonecelular', 'Telefone Celular', $objCAE->enttelefonecelular, array('class' => 'telefone', $disabled));
    	echo $simec->email('entemail', 'E-mail', $objCAE->entemail, array('class' => 'email', $disabled, 'required'));

    	Par3_Controller_Entidade::formEnderecoEntidadePresidente($disabled, $objCAE);

    	if (!empty($objCAE->arqid)) {
    		$link = "<a style='padding: 0px;' class='btn' href='par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$_REQUEST['inuid']}&menu=cae&req=downloadArquivo&arqid={$objCAE->arqid}' target='blank'><small style='font-size: 12px;'><i class='fa fa-download'></i> Abrir Anexo</small></a> | ";
    		$link.= "<a style='padding: 0px;' class='btn' href='par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$_REQUEST['inuid']}&menu=cae&req=excluirArquivo&arqid={$objCAE->arqid}&tparquivo=presidente&capid={$objCAE->capid}'><small style='font-size: 12px;'> <i class='fa fa-trash'></i> Excluir</small></a>";
            echo '<div class="form-group">
                <label for="arqid" class="col-sm-3 col-md-3 col-lg-3 control-label">Anexo I: </label>
                <div class="col-sm-9 col-md-9 col-lg-9"><div class="input-group" style="margin-top: 4px">'.$link.'</div></div>
            </div>';
    	} else {
    		echo $simec->input('arqid', 'Anexo II', null, array('type' => 'file', 'required', 'help' => 'Ata de eleição do Presidente e do Vice-Presidente do CAE'));
    	}
    }//end formDadosCAE()

    /**
     * Função formConselheiro
     * - monta o formulario de dados de pessoa fisica conselheiro
     *
     * @return string escreve a lista.
     *
     */
    public function formConselheiro($disabled = 'disabled', $objPessoaFisica = null, $form = null){
    	global $simec;

        $radioFuncao    = array('M' => 'Membro', 'V' => 'Vice-Presidente');
        $radioTipo      = array('T' => 'Titular', 'S' => 'Suplente');

    	echo '<input type="hidden" id="form" value="'.$form.'"/>';
    	echo '<input type="hidden" name="tenid" value="'.Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_CONSELHEIRO_CAE.'"/>';
    	echo '<input type="hidden" name="entcpf_old" id="entcpf_old-conselheiro" value="'.formatar_cpf($objPessoaFisica->entcpf).'"/>';

        echo $simec->cpf('entcpf', 'CPF', $objPessoaFisica->entcpf, array('id' => 'entcpf-conselheiro','class' => 'cpf entcpfconselheiro', $disabled, 'required', 'data-pessoa' => true, 'data-pessoa-campos' => '{".entnomeconselheiro": "no_pessoa_rf"}'));
    	echo $simec->input('entnome', 'Nome', $objPessoaFisica->entnome, array('id' => 'entnome-conselheiro', 'class' => 'entnomeconselheiro', 'maxlength' => '255', $disabled, 'required', 'readonly' => 'readonly'));

        echo $simec->radio('cacfuncao', 'Função', $objPessoaFisica->cacfuncao ? $objPessoaFisica->cacfuncao : 'M', $radioFuncao, array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
        echo $simec->radio('cactipoocupacao', 'Tipo', $objPessoaFisica->cactipoocupacao ? $objPessoaFisica->cactipoocupacao : 'T', $radioTipo, array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));

    	echo $simec->email('entemail', 'E-mail', $objPessoaFisica->entemail, array('id' => 'entemail-conselheiro', 'class' => 'email', $disabled, 'required'));

        if( !empty($objPessoaFisica->arqid) ){
            $link = "<a style='padding: 0px;' class='btn' href='par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$_REQUEST['inuid']}&menu=cae&req=downloadAta&arqid={$objPessoaFisica->arqid}' target='blank'><small style='font-size: 12px;'><i class='fa fa-download'></i> Abrir Anexo</small></a> | ";
            $link.= "<a style='padding: 0px;' class='btn excluir_arq' href='par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$_REQUEST['inuid']}&menu=cae&req=excluirArquivo&arqid={$objPessoaFisica->arqid}&caeid={$objPessoaFisica->caeid}&cacid={$objPessoaFisica->cacid}'><small style='font-size: 12px;'><i class='fa fa-trash'></i> Excluir</small></a>";
            echo '
                <div class="form-group">
                    <label for="arqid" class="col-sm-3 col-md-3 col-lg-3 control-label">Anexo I: </label>
                        <div class="col-sm-9 col-md-9 col-lg-9 arqid-conselheiro-sx"><div class="input-group" style="margin-top: 4px">'.$link.'</div>
                </div>
            </div>';
        } else {
            echo $simec->input('arqid', 'Ata de indicação do representante ou Ofício', null, ['type' => 'file', 'help' => 'Ata de indicação do representante ou Ofício', 'required']);
        }
    }//end formConselheiro()

    /**
     * Função formConselheiro
     * - monta o formulario de dados de pessoa fisica conselheiro
     *
     * @return string escreve a lista.
     *
     */
    public function formConselheiroExterno($disabled = 'disabled', $objPessoaFisica = null, $form = null) {
    	global $simec;

    	$sqlCategoria = Par3_Model_CAEConselheiroCategoria::pegarSQLSelectCombo();

		if(empty($objPessoaFisica)){
			$objPessoaFisica = new stdClass();
		}

    	$radioBoolean = array('S' => 'Sim', 'N' => 'Não');
    	$radioSexo = array('F' => 'Feminino', 'M' => 'Masculino');
    	$radioFuncao = array('P' => 'Presidente', 'V' => 'Vice-Presidente', 'M' => 'Membro');

    	$selectNiveisEnsino = array(
    		'1' => 'Ensino fundamental',
    		'2' => 'Ensino médio',
    		'3' => 'Ensino superior incompleto',
    		'4' => 'Ensino superior completo',
    		'5' => 'Especialização',
    		'6' => 'Mestrado',
    		'7' => 'Doutorado'
    	);

    	$selectOcupacao = array(
    		'C' => 'Comerciante',
    		'S' => 'Servidor (a) público (a)',
    		'A' => 'Aposentado (a)',
    		'T' => 'Autônomo (a)',
    		'E' => 'Estudante',
    		'D' => 'Desempregado (a)',
    		'O' => 'Outra'
    	);

    	$selectConselho = array(
    		'N' => 'Não participo.',
    		'E' => 'Conselho de Educação',
    		'A' => 'Conselho de Assistência Social',
    		'T' => 'Conselho Tutelar',
    		'U' => 'Conselho de Saúde',
    		'O' => 'Outro'
    	);

    	$selectRenda = array(
    		'1' => 'Até 1 salário mínimo',
    		'3' => 'De 1 a 3 salários mínimos',
    		'6' => 'De 3 a 6 salários mínimos',
    		'9' => 'De 7 a 9 salários mínimos',
    		'10' => 'Acima de 10 salários mínimos'
    	);

    	echo '<input type="hidden" name="caeid" value="'.$objPessoaFisica->caeid.'"/>';
    	echo '<input type="hidden" name="cacid" value="'.$objPessoaFisica->cacid.'"/>';
    	echo '<input type="hidden" name="entid" value="'.$objPessoaFisica->entid.'"/>';
    	echo '<input type="hidden" name="tenid" value="'.Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_CONSELHEIRO_CAE.'"/>';
    	echo '<input type="hidden" id="form" value="'.$form.'"/>';
    	echo '<input type="hidden" name="entcpf_old" value="'.formatar_cpf($objPessoaFisica->entcpf).'"/>';
    	echo $simec->cpf('entcpf', 'CPF', $objPessoaFisica->entcpf, array('class' => 'cpf cpf_conselheiro', $disabled, 'data-pessoa' => true, 'data-pessoa-campos' => '{"#entnome": "no_pessoa_rf"}'));

    	echo $simec->input('entnome', 'Nome', $objPessoaFisica->entnome, array('class' => 'nome_conselheiro', 'maxlength' => '255', $disabled, 'required', 'readonly' => 'readonly'));
    	echo $simec->data('entdtnascimento', 'Data Nascimento', $objPessoaFisica->entdtnascimento, array($disabled, 'required'));
    	echo $simec->telefone('enttelefonecomercial', 'Telefone Principal', $objPessoaFisica->enttelefonecomercial, array('class' => 'telefone', 'required', $disabled));
    	echo $simec->telefone('enttelefonecelular', 'Telefone Alternativo', $objPessoaFisica->enttelefonecelular, array('class' => 'telefone', $disabled));
    	echo $simec->email('entemail', 'E-mail', $objPessoaFisica->entemail, array('class' => 'email', $disabled, 'required'));
    	echo $simec->radio('entsexo', 'Sexo', $objPessoaFisica->entsexo ? $objPessoaFisica->entsexo : 'M', $radioSexo, array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
    	echo $simec->radio('cacfuncao', 'Função', $objPessoaFisica->cacfuncao ? $objPessoaFisica->cacfuncao : 'M', $radioFuncao, array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
    	echo $simec->select('cctid', 'Categoria', $objPessoaFisica->cctid, $sqlCategoria, array($disabled, 'required'));
    	echo $simec->select('cscid', 'Categoria Especializada', $objPessoaFisica->cscid, array(), array($disabled), array('visible' => false));
    	echo $simec->select('entnivelensino', 'Escolaridade', $objPessoaFisica->entnivelensino, $selectNiveisEnsino, array('maxlength' => '255', $disabled, 'required'));
    	echo $simec->radio('cacmandato', 'Mandato?', $objPessoaFisica->cacmandato ? $objPessoaFisica->cacmandato : 'N', $radioBoolean, array($disabled, 'required', 'help' => '1º mandato como Conselheiro de Alimentação Escolar?'), array('type' => 'radio radio-info radio', 'style' => ''));
    	echo $simec->select('cacoutroconselho', 'Outro conselho', $objPessoaFisica->cacoutroconselho, $selectConselho, array($disabled, 'required', 'placeholder' => 'Participa de outro conselho de controle social?'));
    	echo $simec->select('cacocupacao', 'Ocupação atual', $objPessoaFisica->cacocupacao, $selectOcupacao, array($disabled, 'required'));
    	echo $simec->select('cacrenda', 'Renda familiar mensal', $objPessoaFisica->cacrenda, $selectRenda, array($disabled, 'required', 'placeholder' => 'Faixa de renda familiar mensal'));
    }//end formConselheiroExterno()


    /**
     * Função salvarConselheiro
     * - salvar informações do CAE.
     *
     * @return void.
     *
     */
    public function salvarConselheiroExterno($arrDados)
    {
    	$url = "par3.php?modulo=principal/cae/conselheiro&acao=A";

    	$entidade 	 = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);
    	$conselheiro = new Par3_Model_CAEConselheiro();  	
    
    	try {
    		$arrDados['cacid']                  = !empty($arrDados['cacid']) ? $arrDados['cacid'] : null;
    		$arrDados['cscid']                  = !empty($arrDados['cscid']) ? $arrDados['cscid'] : null;
    		$arrDados['entcpf']                 = str_replace(array('.','-','/'), '', $arrDados['entcpf']);
    		$arrDados['enttelefonecelular']     = str_replace(array('(','-',')'), '', $arrDados['enttelefonecelular']);
    		$arrDados['enttelefonecomercial']   = str_replace(array('(','-',')'), '', $arrDados['enttelefonecomercial']);
    		$arrDados['entusucpfinclusao']      = $_SESSION['usucpf'];
    		$arrDados['entdtnascimento']        = $arrDados['entdtnascimento'] != '' ? $arrDados['entdtnascimento'] : null;
    		$arrDados['entstatus']              = 'A';
            $arrDados['inuid']                  = $_SESSION['par']['inuid'];
                
                
                $dia_nasc = substr($arrDados['entdtnascimento'],0,2);
                $mes_nasc = substr($arrDados['entdtnascimento'],3,2);
                $ano_nasc = substr($arrDados['entdtnascimento'],6,4);
                 
                if(checkdate($mes_nasc, $dia_nasc , $ano_nasc) ){
   	               
			    		$entidade->popularDadosObjeto($arrDados);
			    		$arrDados['entid'] = $entidade->salvar();
			
			    		$conselheiro->popularDadosObjeto($arrDados);
			    		$conselheiro->salvar();
			    		$conselheiro->commit();
			
			    		simec_redirecionar($url, 'success');
		    		
                }else{
                   echo "<script>alert('Data Nascimento inválida')</script>";
                }
                
    	} catch (Exception $e) {
    		$conselheiro->rollback();
    		simec_redirecionar($url, 'error', $e->getMessage());
    	}
    }//end salvarConselheiroExterno()


    /**
     * Função salvarCAE
     * - salvar informações do CAE.
     *
     * @return void.
     *
     */
    public function salvarCAE($arrDados, $arrFiles)
    {
    	$url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A" .
    			"&inuid={$arrDados['inuid']}&menu=cae";

    	$endereco = new Par3_Model_Endereco($arrDados['endid']);
    	$entidade = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);
    	$cae 	  = new Par3_Model_CAE($arrDados['caeid']);

    	try {
    		$arrDados['entcpf']			  	  = str_replace(array('.','-','/'), '', $arrDados['entcpf']);
    		$arrDados['endcep'] 			  = str_replace(array('.','-'), '', $arrDados['endcep']);
    		$arrDados['enttelefonecomercial'] = str_replace(array('(','-',')'), '', $arrDados['enttelefonecomercial']);
    		$arrDados['entstatus'] 			  = 'A';
    		$arrDados['arqid'] 			  	  = empty($arrDados['arqid']) ? null : $arrDados['arqid'];
    		$arrDados['entid'] 			  	  = empty($arrDados['entid']) ? null : $arrDados['entid'];

    		if ($arrDados['entcpf_old'] != $arrDados['entcpf']) {
                $arrDados['entusucpfinclusao'] = $_SESSION['usucpf'];
    			$entidade->inativaEntidadesInuidPorTipo($arrDados['inuid'], $arrDados['tenid']);

    			unset($arrDados['entid']);

    			$cae = new Par3_Model_CAE();
    			$entidade = new Par3_Model_InstrumentoUnidadeEntidade();
    		}

    		if (!empty($arrFiles['arqid'])) {
    			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    			$file = new FilesSimec("cae", null, "par3");
    			$file->setUpload('Decreto de Nomeação', 'arqid', false);
    			$arrDados['arqid'] = $file->getIdArquivo();
    		}

    		$endereco->popularDadosObjeto($arrDados);
    		$arrDados['endid'] = $endereco->salvar();

    		$entidade->popularDadosObjeto($arrDados);
    		$arrDados['entid'] = $entidade->salvar();

    		$cae->popularDadosObjeto($arrDados);
    		$cae->salvar();

    		$cae->commit();

    		simec_redirecionar($url, 'success');
    	} catch (Exception $e) {
    		$cae->rollback();
    		simec_redirecionar($url, 'error', $e->getMessage());
    	}
    }//end salvarCAE()

    /**
     * Função salvarPresidente
     * - salvar informações do CAE.
     *
     * @return void.
     *
     */
    public function salvarPresidente($arrDados, $arrFiles){

    	$url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrDados['inuid']}&menu=cae";

    	$endereco   = new Par3_Model_Endereco($arrDados['endid']);
    	$entidade   = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);
    	$presidente = new Par3_Model_CAEPresidente($arrDados['capid']);

        $caeConselheiro = new Par3_Model_CAEConselheiro();

    	try {
    		$entid = $entidade->recuperarEntidadeUnidadePorCPF($arrDados['inuid'], $arrDados['entcpf'], $arrDados['tenid']);

    		$arrDados['entcpf']                 = str_replace(array('.','-','/'), '', $arrDados['entcpf']);
    		$arrDados['endcep']                 = str_replace(array('.','-'), '', $arrDados['endcep']);
    		$arrDados['enttelefonecelular']     = str_replace(array('(','-',')'), '', $arrDados['enttelefonecelular']);
    		$arrDados['enttelefonecomercial']   = str_replace(array('(','-',')'), '', $arrDados['enttelefonecomercial']);
    		$arrDados['entstatus']              = 'A';
    		$arrDados['entusucpfinclusao']      = $_SESSION['usucpf'];
    		$arrDados['entid']                  = $entid ? $entid : null;

    		if ($arrDados['entcpf_old'] != $arrDados['entcpf']) {
                    $arrDados['entusucpfinclusao'] = $_SESSION['usucpf'];
                    $entidade->inativaEntidadesInuidPorTipo($arrDados['inuid'], $arrDados['tenid']);
                    unset($arrDados['entid']);
                    $presidente  = new Par3_Model_CAEPresidente();
                    $entidade = new Par3_Model_InstrumentoUnidadeEntidade();
    		}

    		if (!empty($arrFiles['arqid'])) {
                    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
                    $file = new FilesSimec("cae_presidente", null, "par3");
                    $file->setUpload('Ata de eleição', 'arqid', false);
                    $arrDados['arqid'] = $file->getIdArquivo();
    		}

//                if( $entid > 0 ){
////                    ver($arrDados, d);
//                    $arrCAE['cacfuncao'] = $arrDados['cacfuncao'];
//                    $arrCAE['cactipoocupacao'] = $arrDados['cactipoocupacao'];
//                    $caeConselheiro->popularDadosObjeto($arrCAE);
//                }

    		$endereco->popularDadosObjeto($arrDados);
    		$arrDados['endid'] = $endereco->salvar();

    		$entidade->popularDadosObjeto($arrDados);
    		$arrDados['entid'] = $entidade->salvar();

    		$presidente->popularDadosObjeto($arrDados);
    		$presidente->salvar();
    		$presidente->commit();

    		simec_redirecionar($url, 'success');
    	} catch (Exception $e) {
    		$conselheiro->rollback();
    		simec_redirecionar($url, 'error', $e->getMessage());
    	}
    }//end salvarPresidente()


    public function buscaAtualizaVicePresidente( $dados ){
        global $db;

        $entcpf     = str_replace('.', '', str_replace('-', '', $dados['entcpf']) );
        $cacfuncao  = trim( $dados['cacfuncao'] );
        $inuid      = trim( $dados['inuid'] );
        $tenid      = trim( $dados['tenid'] );

        $sql = "
            SELECT  sub.cacid

            FROM par3.instrumentounidade_entidade ent

            INNER JOIN par3.instrumentounidade  inu ON inu.inuid  = ent.inuid
            LEFT JOIN (
                select cacid, caeid, entid, cacfuncao, cactipoocupacao, cc.cctdsc, max(arqid) AS arqid
                from par3.cae_conselheiro AS c
                left join par3.cae_conselheiro_categoria as cc on cc.cctid = c.cctid
                group by cacid, caeid, entid, arqid, cacfuncao, cactipoocupacao, cc.cctdsc
            ) AS sub ON sub.entid = ent.entid

            WHERE ent.tenid = {$tenid} AND inu.inuid = {$inuid} AND ent.entstatus = 'A'  AND sub.cacfuncao = '{$cacfuncao}' AND entcpf <> '{$entcpf}'
        ";
        $cacid = $db->carregar($sql);

        if( $cacid[0] > 0 ){
            foreach( $cacid AS $dados ){
                $sql = "UPDATE par3.cae_conselheiro SET cacfuncao = 'M' WHERE cacid = {$dados['cacid']};";
                $db->executar($sql);
            }
        }
    }

    /**
     * Função salvarConselheiro
     * - salvar informações do CAE.
     *
     * @return void.
     *
     */
    public function salvarConselheiro($arrDados){
    	$url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrDados['inuid']}&menu=cae";

    	if( !$arrDados['entcpf'] || !$arrDados['entnome'] || !$arrDados['entemail'] ){
            simec_redirecionar($url, 'error', 'Dados incompletos');
    	}

        $conselheiro = new Par3_Model_CAEConselheiro($arrDados['cacid']);
        $usuResponsabilidade = new Par3_Model_UsuarioResponsabilidade();

        $dadosUsuResponsabilidade = $usuResponsabilidade->retornaUsuarioResponsabilidade(preg_replace('![^0-9]!', '', $arrDados['entcpf']), Par3_Model_UsuarioResponsabilidade::CONSELHEIRO);

        if($arrDados['cacfuncao']){
            $vice = $this->buscaAtualizaVicePresidente($arrDados);
        }

        $updateConselheiro = false;
        if (isset($_GET['req']) && $_GET['req'] == 'editarEntidade') {
            $arrDados['entid'] = $_GET['entid'];
            $conselheiro->recuperaPorCaeId($arrDados['caeid'], $arrDados['entid']);
            $updateConselheiro = true;
        } else {
            $existeConselheiro = $conselheiro->existeConselheiro(preg_replace('![^0-9]!', '', $arrDados['entcpf']), $arrDados['inuid']);

            if( $existeConselheiro ){
                $conselheiro->recuperaPorCaeId($existeConselheiro[0]['caeid'], $existeConselheiro[0]['entid']);
            }

            if( !empty($conselheiro->arAtributos['cacid']) ){
                $arrDados['entid'] = $existeConselheiro[0]['entid'];
                $updateConselheiro = true;
            }
        }

        $unidade    = new Par3_Controller_InstrumentoUnidade();
        $entidade   = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);
        $usuario    = new Seguranca_Model_Usuario();
        $email      = true;

        if (count($_FILES)) {
            require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
            $file = new FilesSimec('arquivo', null, 'public');
            $file->setUpload('Ato de nomeação', 'arqid', false);

            if (is_object($file) && method_exists($file, 'getIdArquivo')) {
                $arrDados['arqid'] = $file->getIdArquivo();
            }
        }

        if ($updateConselheiro) {
            try {
                $arrDados['entcpf'] = preg_replace('![^0-9]!', '', $arrDados['entcpf']);
                $entidade->popularDadosObjeto($arrDados);
                $entidade->salvar();
                $entidade->commit();

                $conselheiro->popularDadosObjeto($arrDados);
                $conselheiro->salvar();
                $conselheiro->commit();

                simec_redirecionar($url, 'success');
            } catch (Exception $e) {
                $entidade->rollback();
                $conselheiro->rollback();
                simec_redirecionar($url, 'error', $e->getMessage());
            }
        }

    	try {
            $arrDados['entcpf']                 = str_replace(array('.','-','/'), '', $arrDados['entcpf']);
            $arrDados['enttelefonecelular']     = str_replace(array('(','-',')'), '', $arrDados['enttelefonecelular']);
            $arrDados['enttelefonecomercial']   = str_replace(array('(','-',')'), '', $arrDados['enttelefonecomercial']);
            $arrDados['entstatus']              = 'A';
            $arrDados['entusucpfinclusao']      = $_SESSION['usucpf'];
            $arrDados['usucpf']                 = $arrDados['entcpf'];
            $arrDados['entid']                  = $entidade->recuperarEntidadeUnidadePorCPF($arrDados['inuid'], $arrDados['entcpf'], $arrDados['tenid']);

            $entidade->popularDadosObjeto($arrDados);

            $arrDados['entid'] = $entidade->salvar();
            $arrDados['cctid'] = null;
            $arrDados['cscid'] = null;

            if (is_object($file) && method_exists($file, 'getIdArquivo')) {
                $arrDados['arqid'] = $file->getIdArquivo();
            }

            $conselheiro->popularDadosObjeto($arrDados);
            $conselheiro->salvar();

            $mun_est_cod = $unidade->buscaEstadoMunicipioUnidade($arrDados['inuid']);
            $esfera = $unidade->buscaEsferaUnidade($arrDados['inuid']);

            $arDadosResponsabilidade['rpuid'] = $dadosUsuResponsabilidade['rpuid'];
            $arDadosResponsabilidade['pflcod'] = Par3_Model_UsuarioResponsabilidade::CONSELHEIRO;
            $arDadosResponsabilidade['usucpf'] = $arrDados['entcpf'];
            $arDadosResponsabilidade['entid'] = $arrDados['entid'];

            if( $esfera == 'M' ){
                $arDadosResponsabilidade['muncod'] = $mun_est_cod;
            }else{
                $arDadosResponsabilidade['estuf'] = $mun_est_cod;
            }

            $usuResponsabilidade->popularDadosObjeto($arDadosResponsabilidade);
            $usuResponsabilidade->salvar();
            $usuResponsabilidade->commit();

            if (empty($arrDados['entcpf']) || empty($arrDados['entnome'])) {
                throw new Exception('CPF ou Nome do conselheiro não informado.');
            }
            if (!$usuario->recuperarPorCPF($arrDados['entcpf'])) {
                $senhageral = 'cae' . substr($arrDados['usucpf'], -3);
                $senha = md5_encrypt_senha($senhageral, '');

                $dadosUsuario['usucpf'] = $arrDados['usucpf'];
                $dadosUsuario['usunome'] = $arrDados['entnome'];
                $dadosUsuario['usuemail'] = $arrDados['entemail'];
                $dadosUsuario['ususenha'] = $senha;
                $dadosUsuario['usuchaveativacao'] = 'F';
                $dadosUsuario['suscod'] = 'A';

                $usuario->popularDadosObjeto($dadosUsuario);
                $usuario->salvar();
                $usuario->commit();

                $dadosUsuario['ususenha'] = $senhageral;
                $dadosUsuario['inuid'] = $arrDados['inuid'];
                $itrid = $unidade->pegarItrid($arrDados['inuid']);
                $dadosUsuario['itrid'] = $itrid;

                $dadosPerfil['usucpf'] =  $arrDados['usucpf'];
                $dadosPerfil['sisid'] =  $_SESSION['sisid'];
                $dadosPerfil['pflcod'] =  Par3_Model_UsuarioResponsabilidade::CONSELHEIRO;

                $perfilUsuario = new Seguranca_Model_Perfilusuario();
                $perfilUsuario->salvarDados($dadosPerfil);

                unset($usuarioPar);

                $usuarioSistema = new Seguranca_Model_Usuariosistema();
                $usuarioPar = $usuarioSistema->recuperarTodos('*', array( "usucpf = '{$arrDados['usucpf']}' AND sisid = {$_SESSION['sisid']} "));

                    
                if (empty($usuarioPar[0])) {
                    $arrayUsuarioSistema['usucpf'] = $arrDados['usucpf'];
                    $arrayUsuarioSistema['sisid']  = $_SESSION['sisid'];
                    $arrayUsuarioSistema['pflcod']  = Par3_Model_UsuarioResponsabilidade::CONSELHEIRO;
                    $usuarioSistema->salvar($arrayUsuarioSistema);
                    $usuarioSistema->commit();
                }

                //enviar email para o primeiro acesso no simec;
                $this->enviarEmailConselheiro($dadosUsuario, 1);
                $email = false;
            } else {
                $usuarioSistema = new Seguranca_Model_Usuariosistema();
                $usuarioPar = $usuarioSistema->recuperarTodos('*', array( "usucpf = '{$arrDados['usucpf']}' AND sisid = {$_SESSION['sisid']} "));

                if (empty($usuarioPar[0])) {
                    $arrayUsuarioSistema['usucpf'] = $arrDados['usucpf'];
                    $arrayUsuarioSistema['sisid']  = $_SESSION['sisid'];
                    $arrayUsuarioSistema['pflcod']  = Par3_Model_UsuarioResponsabilidade::CONSELHEIRO;
                    $usuarioSistema->salvar($arrayUsuarioSistema);
                    $usuarioSistema->commit();
                }

                unset($usuarioPar);

                $perfilUsuario = new Seguranca_Model_Perfilusuario();
                $usuarioPar = $perfilUsuario->recuperarTodos('*', array( "usucpf = '{$arrDados['usucpf']}' AND pflcod = ".Par3_Model_UsuarioResponsabilidade::CONSELHEIRO));

                if(empty($usuarioPar[0])) {
                    $arrayPerfilUsuario['usucpf']   = $arrDados['usucpf'];
                    $arrayPerfilUsuario['pflcod']   = Par3_Model_UsuarioResponsabilidade::CONSELHEIRO;
                    $arrayPerfilUsuario['sisid']    =  $_SESSION['sisid'];
                    $perfilUsuario->salvarDados($arrayPerfilUsuario);
                }
            }

            $conselheiro->commit();

            //Busca a siatuação do usuario recem criado
            $dadoUsuario = $usuario->recuperarTodos('suscod', ["usucpf = '{$arrDados['usucpf']}'"]);
            $suscod = ($dadoUsuario[0]['suscod']) ? $dadoUsuario[0]['suscod'] : 'P';
            
            $result = $usuResponsabilidade->buscarDadosParaHistoricoUsuario( $arDadosResponsabilidade );
      
            //cria um historico de atribuição de perfil
            $usuario->cadastrarHistorico([
                'htudsc' => sprintf("Cadastramento como {$result['perfil']} via interface PAR, realizado por {$result['nome']} - CPF: %s, com o perfil {$result['pf_resp']},  associado à: {$result['local']}, na da data de {$result['hoje']}", $_SESSION['usucpf']),
                'usucpf' => $arrDados['usucpf'],
                'sisid'  => PAR3_SIS_ID,
                'suscod' => $suscod,
                'usucpfadm' => $_SESSION['usucpf']
            ]);

            if ($email) {
                $this->enviarEmailConselheiro($arrDados, 0);
            }

            simec_redirecionar($url, 'success');
    	} catch (Exception $e) {
            $conselheiro->rollback();
            simec_redirecionar($url, 'error', $e->getMessage());
    	}
    }//end salvarConselheiro()


    /**
     * Função verificaConselheiro
     * - verfifica se o conselheiro já é cadastrado em outra unidade
     *
     * @param integer $cpf
     *
     * @return string $mensagem.
     *
     */
    public function verificaConselheiro($cpf)
    {
    	$model = new Par3_Model_CAEConselheiro();
    	$conselheiro = $model->carregarConselheiroCPF($cpf);

    	$locais = '';

    	if (is_array($conselheiro)) {
    		foreach ($conselheiro as $k => $local) {
    			$separador = ', ';
    			if ($k === count($conselheiro)-2) $separador = ' e ';
    			if ($k === count($conselheiro)-1) $separador = '';
    			$locais = $local['inudescricao'].$separador;
    		}
    	}

    	$mensagem = null;

    	if ($conselheiro[0]['entnome'] != '') {
    		$mensagem = "O conselheiro <b>{$conselheiro[0]['entnome']}</b> selecionado
    		está vinculado em <b>$locais</b>.
    		Deseja fazer o cadastro dele em um novo município!";
    	}

    	return $mensagem;
    }//end verificaConselheiro()

    public function verificaPreenchimentoCAE($inuid)
    {
    	$modelCAE = new Par3_Model_CAE();
    	$conselho = $modelCAE->carregarDadosCAE($inuid);
    	$presidente = $modelCAE->carregarDadosPresidenteCAE($inuid);
    	$conselheiros = $modelCAE->carregarConselheirosCAE($inuid);

        $perc = 0;

    	if (!empty($conselho->arqid)) $perc++;
    	if (!empty($presidente->capid)) $perc++;

        $qtd = null;
        $intTotalArqid = 0;
    	if (is_array($conselheiros)) {
    		foreach ($conselheiros as $dados) {
    			if ($dados['entcpf'] != null ||
                    $dados['entnome'] != null ||
                    $dados['entemail'] != null ||
                    $dados['arqid'] != null) {
    				$qtd++;
    			}

                if ($dados['arqid']) {
                    $intTotalArqid++;
                }
    		}
        	if ($qtd>=6) $perc++;
    	}

        $intTotalConselheiros = count($conselheiros);
        if (($intTotalConselheiros > 0) && ($intTotalArqid != $intTotalConselheiros)) {
            --$perc;
        }

        if ($perc > 0) $perc = $perc / 3 * 100;

        return (int)$perc;
    }

    /**
     * Função inativar conselheiro
     * - inativa membro do conselho.
     *
     * @return void.
     *
     */
    public function inativaCAE($arrDados)
    {
    	$url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A" .
    			"&inuid={$arrDados['inuid']}&menu=cae";

    	$conselheiro = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);

    	try {
    		$conselheiro->entstatus = 'I';
    		$conselheiro->entdtinativacao = date('Y-m-d H:i:s');
    		$conselheiro->entusucpfinativacao = $_SESSION['usucpf'];
    		$conselheiro->salvar();
    		$conselheiro->commit();

    		simec_redirecionar($url, 'success');
    		die();
    	} catch (Exception $e) {
    		$conselheiro->rollback();
    		simec_redirecionar($url, 'error');
    		die();
    	}
    }//end inativarConselheiro()

    public function getConselheiro($arrDados){
        global $db;

        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrDados['inuid']}&menu=cae";

        $conselheiro = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);
        $oRetorno = new stdClass();

        $arquivo = $db->pegaLinha('select cacid, caeid, cacfuncao, cactipoocupacao, arqid from par3.cae_conselheiro where entid = '.$arrDados['entid'].' order by cacid desc limit 1');
        if ($arquivo) {
            $oRetorno->arqid            = $arquivo['arqid'];
            $oRetorno->cacid            = $arquivo['cacid'];
            $oRetorno->caeid            = $arquivo['caeid'];
            $oRetorno->cacfuncao        = $arquivo['cacfuncao'];
            $oRetorno->cactipoocupacao  = $arquivo['cactipoocupacao'];
        }

        $oRetorno->entcpf   = $conselheiro->entcpf;
        $oRetorno->entnome  = $conselheiro->entnome;
        $oRetorno->entemail = $conselheiro->entemail;

        return ($conselheiro) ? $oRetorno : false;
    }

    /**
     * Função de download de arquivo
     * - downloadArquivo
     *
     * @return void.
     *
     */
    public function downloadArquivo($arqid) {
    	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    	$file = new FilesSimec("cae", null, "par3");
    	$file->getDownloadArquivo($arqid);
    }//end downloadArquivo()


    /**
     * Função de excluir arquivo
     * - excluirArquivo
     *
     * @return void.
     *
     */
    public function excluirArquivo($arrDados)
    {
    	include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

    	$url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A' .
    			"&inuid={$arrDados['inuid']}&menu=cae";

    	$file = new FilesSimec('cae', null, 'par3');

    	try {
	    	if ($arrDados['tparquivo'] == 'cae') {
                $model = new Par3_Model_CAE($arrDados['caeid']);
                $model->arqid = null;
                $model->alterar(array('arqid'));
            } elseif ($arrDados['tparquivo'] == 'presidente') {
                $model = new Par3_Model_CAEPresidente($arrDados['capid']);
                $model->arqid = null;
                $model->alterar(['arqid']);
	    	} else {
	    		$model = new Par3_Model_CAEConselheiro($arrDados['cacid']);
	    		$model->arqid = null;
	    		$model->alterar(array('arqid'));
	    	}

	    	$model->commit();
	    	$file->excluiArquivoFisico($arrDados['arqid']);
	    	simec_redirecionar($url, 'success');
	    	die();
    	} catch (Exception $e) {
    		$model->rollback();
    		simec_redirecionar($url, 'error');
    		die();
    	}
    }//end excluirArquivo()

    /**
     * Função enviarEmailConselheiro
     * - envio de email.
     *
     * @return void.
     *
     */
    public function enviarEmailConselheiro($arrDados, $novo)
    {
    	$controleUnidade = new Par3_Controller_InstrumentoUnidade();
    	$descricao = $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']);

    	if ($arrDados['itrid' == '2']) {
    		$municipio = $descricao;
    		$tipoEsfera = 'Município';
    	} else {
    		$tipoEsfera = 'Estado';
    	}

		$unidade = $descricao;
		$assunto =  "Cadastro de Conselheiro CAE " . $descricao ;

    	if ($novo) {
    		$mensagem =
    		"
    		<pre align=\"center\" style=\"text-align: justify;\"  >
    		Você foi cadastrado como Conselheiro no âmbito do Conselho de Alimentação Escolar no {$tipoEsfera} de {$unidade}.
    		Por favor, acesse o link a seguir para acessar o SIMEC/Módulo PAR, insira seu CPF, senha e clique no botão <span style=\"width: 100px; color: white; background-color:#47A447; border: 0px;\"  >  Entrar  </span>. Confirme que
    		você atua nesta função clicando no botão \"Validar\".  Caso não atue nesta função, escolha a opção \"Não validar\".

    		http://simec.mec.gov.br
    		Usuário: {$arrDados['usucpf']}
    		Senha: {$arrDados['ususenha']}

    		Atenciosamente,
    		Equipe SIMEC/PAR.
    		</pre>";
    	} else {
    		$mensagem =
    		"
    		<pre align=\"center\" style=\"text-align: justify;\"  >
    		Você foi cadastrado como Conselheiro no âmbito do Conselho de Alimentação Escolar no {$tipoEsfera} de {$unidade}.
    		Por favor, acesse o link a seguir para acessar o SIMEC/Módulo PAR, insira seu CPF, senha atuais de cadastro no SIMEC e clique no botão <span style=\"width: 100px; color: white; background-color:#47A447; border: 0px;\"  >  Entrar  </span>. Confirme que
    		você atua nesta função clicando no botão \"Validar\".  Caso não atue nesta função, escolha a opção \"Não validar\".

    		Caso não lembre a sua senha acesse o link http://simec.mec.gov.br/recupera_senha.php
    		Se mesmo assim não conseguir acessar o sistema, entre em contato com:

    		Equipe Técnica do PAR  Plano de Ações Articuladas

    		Secretaria de Educação Básica
    		Ministério da Educação
    		Telefones: (61) 2022-8334/8337 / 8338/8332/9448.
    		Endereço eletrônico: planodemetas@mec.gov.br

    		http://simec.mec.gov.br

    		Atenciosamente,
    		Equipe SIMEC/PAR.
    		</pre>";
    	}

    	$remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
    	$mensagem = html_entity_decode($mensagem);
    	$to = $arrDados['entemail'];

    	$retorno = enviar_email($remetente, $to, $assunto, $mensagem);
    }//end enviarEmailConselheiro()

    public function verificaVicePresidente( $post ){
        global $db;
        $entcpf     = str_replace('.', '', str_replace('-', '', $post['entcpf']) );
        $cacfuncao  = trim( $post['cacfuncao'] );
        $inuid      = trim( $post['inuid'] );
        $tenid      = trim( $post['tenid'] );

        $sql = "
            SELECT  sub.cacid

            FROM par3.instrumentounidade_entidade ent

            INNER JOIN par3.instrumentounidade  inu ON inu.inuid  = ent.inuid
            LEFT JOIN (
                select cacid, caeid, entid, cacfuncao, cactipoocupacao, cc.cctdsc, max(arqid) AS arqid
                from par3.cae_conselheiro AS c
                left join par3.cae_conselheiro_categoria as cc on cc.cctid = c.cctid
                group by cacid, caeid, entid, arqid, cacfuncao, cactipoocupacao, cc.cctdsc
            ) AS sub ON sub.entid = ent.entid

            WHERE ent.tenid = {$tenid} AND inu.inuid = {$inuid} AND ent.entstatus = 'A'  AND sub.cacfuncao = '{$cacfuncao}' AND entcpf <> '{$entcpf}'
        ";
        $cacid = $db->pegaUm($sql);

        if( $cacid > 0 ){
            echo 'S';
        }else{
            echo 'N';
        }
    }

    public function listarCae($arrPost)
    {
            $modelCae = new Par3_Model_CAE();
            $sql = $modelCae->getSqlListaCae($arrPost);

            $cabecalho = array('Mudar Estado', 'CPF', 'Nome', 'E-mail', 'Arquivo*', 'Estado Atual');
            $listagem = new Simec_Listagem();
            $listagem->setCabecalho($cabecalho)
                    ->setQuery($sql)
                    ->addCallbackDeCampo('entid', 'addCheckbox')
                    ->addCallbackDeCampo('arqid', 'setaLinkArquivo')
                    ->addCallbackDeCampo('entcpf', 'formatar_cpf') // função php para formatar cpf
                    ->addAcao('view', 'visualizarCae')
                    ->setTamanhoPagina(30)
                    ->render();
    }

    public function detalhamentoCae($entid){
            include APPRAIZ . 'par3/modulos/principal/acompanhamento/cae/detalheCae.php';
    }


    public function formDadosGerais($dados, $disabled){
            global $simec;
            if($disabled){

            }

            $radioSexo = array('' => 'Ambos', 'F' => 'Feminino', 'M' => 'Masculino');

            echo $simec->input('entnome', 'Nome', $dados['entnome'], array('maxlength' => '255'));
            echo $simec->email('entemail', 'E-mail', $dados['entemail'], array('class' => 'email', false));
            echo $simec->cpf('entcpf', 'CPF', $dados['entcpf'], array('class' => 'cpf'));

            $sql = " select inuid as codigo, inudescricao as descricao from par3.instrumentounidade where itrid = 1 or itrid = 3 ORDER BY inudescricao  ASC";
//		echo $simec->select("estuf", 'UF (Secretaria estadual)', $dados['estuf'], $sql, array('maxlength' => '255', null));

            $sql = Territorios_Model_Estado::pegarSQLSelect($_POST);
            $arrAttr = ['data-placeholder' => 'Unidade Federativa'];
            echo $simec->select('estuf[]', 'UF (Secretaria estadual)', $_POST['estuf'], $sql, $arrAttr);

            $muncods = $_POST['muncods'];
            $arrAttr = ['placeHolder' => 'Municipio'];
            echo $simec->select('muncods[]', 'Municípios', $muncods, $arrAttr);

//		echo $simec->Municipios("listaMunicipio[]", 'Municípios', $dados['listaMunicipio'], "", "");

            echo $simec->data('entdtnascimento', 'Data Nascimento', $dados['entdtnascimento']);
            echo $simec->telefone('enttelefonecomercial', 'Telefone CAE', $dados['enttelefonecomercial'], array('class' => 'telefone', false));

            echo $simec->radio('entsexo', 'Sexo', $dados['entsexo'],$radioSexo , array(), array('type' => 'radio radio-info radio', 'style' => 'inline'));
    }

    public function formDadosConselho($dados, $disabled){
            global $simec;

            $sqlCategoria = Par3_Model_CAEConselheiroCategoria::pegarSQLSelectCombo();
            $radioBoolean = array('' => 'Ambos', 'S' => 'Sim', 'N' => 'Não');

            $radioFuncao = array('' => 'Ambos', 'P' => 'Presidente', 'V' => 'Vice-Presidente', 'M' => 'Membro');
            $selectNiveisEnsino = array(
                    '' =>'Selecione',
                    '1' => 'Ensino fundamental',
                    '2' => 'Ensino médio',
                    '3' => 'Ensino superior incompleto',
                    '4' => 'Ensino superior completo',
                    '5' => 'Especialização',
                    '6' => 'Mestrado',
                    '7' => 'Doutorado'
            );
            $selectOcupacao = array(
                    '' =>'Selecione',
                    'C' => 'Comerciante',
                    'S' => 'Servidor (a) público (a)',
                    'A' => 'Aposentado (a)',
                    'T' => 'Autônomo (a)',
                    'E' => 'Estudante',
                    'D' => 'Desempregado (a)',
                    'O' => 'Outra'
            );
            $selectConselho = array(
                    '' =>'Selecione',
                    'N' => 'Não participo.',
                    'E' => 'Conselho de Educação',
                    'A' => 'Conselho de Assistência Social',
                    'T' => 'Conselho Tutelar',
                    'U' => 'Conselho de Saúde',
                    'O' => 'Outro'
            );

            echo $simec->radio('cacfuncao', 'Função', $dados['cacfuncao'], $radioFuncao, array(), array('type' => 'radio radio-info radio', 'style' => 'inline'));
            echo $simec->select('cctid', 'Categoria', $dados['cctid'], $sqlCategoria);
            echo $simec->select('cscid', 'Categoria Especializada', $dados['cscid'], array(), array($disabled), array('visible' => false));
            echo $simec->select('entnivelensino', 'Escolaridade', $dados['entnivelensino'], $selectNiveisEnsino, array('maxlength' => '255'));
            echo $simec->radio('cacmandato', 'Mandato?', $dados['cacmandato'], $radioBoolean, array('help' => '1º mandato como Conselheiro de Alimentação Escolar?'), array('type' => 'radio radio-info radio', 'style' => ''));
            echo $simec->select('cacoutroconselho', 'Outro conselho',$dados['cacoutroconselho'], $selectConselho, array('placeholder' => 'Participa de outro conselho de controle social?'));
            echo $simec->select('cacocupacao', 'Ocupação atual', $dados['cacocupacao'], $selectOcupacao);

            echo $simec->input('caedocumento', 'Nº Documento', $dados['caedocumento'], array('maxlength' => '255', false));
//		echo $simec->radio('caenomeacao', 'Tipo de Nomeação', $dados['caenomeacao'], array('P' => 'Portaria', 'D' => 'Decreto', 'O' => 'Outros', '' => 'Todos'), false, array('type' => 'radio radio-info radio', 'style' => 'inline'));
            echo $simec->radio('anexo', 'Com Anexo', $dados['anexo'], $radioBoolean, array(), array('type' => 'radio radio-info radio', 'style' => 'inline'));
            echo $simec->radio('caestatus', 'Situação', $dados['caestatus'], $radioBoolean, array(), array('type' => 'radio radio-info radio', 'style' => 'inline'));
    }
}



?>
