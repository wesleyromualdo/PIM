<?php

include APPRAIZ_ZEND . 'Validate/Digits.php';
include APPRAIZ_ZEND . 'Validate/NotEmpty.php';
include APPRAIZ_ZEND . 'Validate/Date.php';
include APPRAIZ_ZEND . 'Validate/StringLength.php';
include APPRAIZ_ZEND . 'Validate/Float.php';

/**
 * Controle responsavel pelas entidades.
 *
 * @author Equipe simec - Consultores OEI
 * @since  14/05/2013
 *
 * @name       Abstract_Model
 * @package    Abstract
 * @version    $Id
 */
abstract class Abstract_Model {

    protected $_db;
    protected $_commit;
    public $entity;
    public $error = null;
    protected $_decode = true;

    const MSG_DATA_INVALIDA = ' não é uma data válida';
    const MSG_DATA_HORA_INVALIDA = ' não se encaixa no formato de Horas (Exemplo: 13:00)';
    const MSG_INTEIRO_INVALIDO = ' ultrapassa o limite definido para este campo';

    public function setDecode($value)
    {
        $this->_decode = $value;
        return $this;
    }

    public function getDecode()
    {
        return $this->__decode;
    }



    public function __construct($commit = true) {
        $this->_names = $this->_schema . '.' . $this->_name;

        global $db;
        $this->_db = $db;
        $this->_commit = $commit;
    }

    public function rollback() {
        $this->_db->rollback();
    }

    public function commit() {
        $this->_db->commit();
    }

    /**
     * Pega os nomes das cahves primarias da tabela.
     *
     * @name getPkNames
     * @access public
     * @return array $pkNames - Array com os nomes das chaves primarias.
     *
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     * @since 22/08/2013
     */
    public function getPkNames() {
        $pkNames = array();
        foreach ($this->entity as $columnName => $column) {
            if ($column['contraint'] == 'pk')
                $pkNames[] = $columnName;
        }

        return $pkNames;
    }

    /**
     * Pega os valores das cahves primarias da tabela.
     *
     * @name getPkValues
     * @access public
     * @return array $pkValues - Array com os valores das chaves primarias, sendo a chave do array o nome da coluna.
     *
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     * @since 22/08/2013
     */
    public function getPkValues() {
        $pkValues = array();
        foreach ($this->entity as $columnName => $column) {
            if (!empty($column['contraint']) && $column['contraint'] === 'pk')
                $pkValues[$columnName] = $column['value'];
        }

        return $pkValues;
    }

    /**
     * Checa se as chaves primarias estao todas com valores.
     *
     * @name checkPkValues
     * @access public
     * @return boolean - true / false
     *
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     * @since 22/08/2013
     */
    public function checkPkValues() {
        foreach ($this->getPkValues() as $pk) {
            if (empty($pk))
                return false;
        }

        return true;
    }

    /**
     * Pega os valores da entidade e retorna um array onde a chave dos valores e o nome da coluna na tabela do banco de dados.
     *
     * @name getEntityValues
     * @access public
     * @return array $entityValues - Array com os valores da entidade e as chaves do array e o nome da coluna.
     *
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     * @since 22/08/2013
     */
    public function getEntityValues($pk = true) {
        $entityValues = array();
        foreach ($this->entity as $columnName => $column) {

            if (($column['contraint'] == 'pk' && $pk == true) || ($column['contraint'] != 'pk')) {
                $entityValues[$columnName] = $column['value'];
            }
        }

        return $entityValues;
    }

    /**
     * Limpa todos os valores da entidade.
     * E utilizado mais quando se necessita salvar mais de uma vez na mesma tabela, fazendo com que nao necessite chamar denovo a mesma model.
     *
     * @name clearEntity
     * @access public
     * @return void
     *
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     * @since 14/08/2013
     */
    public function clearEntity() {
        foreach ($this->entity as $key => &$entity)
            $this->entity[$key]['value'] = "";
    }

    /**
     * Coloca todos os valores na entidade de acordo com a chave do array passado para este metodo.
     *
     * @name populateEntity
     * @param array $data - Valores para preencher a entity
     * @access public
     * @return void
     *
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     * @since 14/08/2013
     */
    public function populateEntity($data = null) {
        //Preenchendo a entity com os dados novos.
        if ($data) {
            foreach ($data as $key => $value) {
                if (isset($this->entity[$key]))
                    $this->entity[$key]['value'] = $value;
            }
        }

        // Se todas as chaves primarias tiverem valor, pega os valores de acordo com as chaves primarias.
        if ($this->checkPkValues()) {

            $entityValuesBD = $this->getByValues($this->getPkValues());
            // Se tiver valores no banco de acordo de acordo com as chaves primarias ele preenche a entity com esses dados.
            if ($entityValuesBD) {
                foreach ($entityValuesBD as $key => $value) {
                    if (isset($this->entity[$key]) && !empty($value))
                        $this->entity[$key]['value'] = $value;
                }
            }
        }

        //Preenchendo a entity com os dados novos.
        if ($data) {
            foreach ($data as $key => $value) {
                if (isset($this->entity[$key]))
                    $this->entity[$key]['value'] = $value;
            }
        }
    }

    /**
     * Trata os valores da entidade, para ficar de acordo com o banco.
     *
     * @name treatEntity
     * @access public
     * @return boolean - true / false
     *
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.com>
     * @since 22/08/2013
     */
    public function treatEntity() {
        foreach ($this->entity as $columnName => &$column) {
            if (($column['contraint'] != 'pk' || $column['is_null'] == 'YES') && empty($column['value'])) {
                $column['value'] = 'NULL';
            } else if (!empty($column['value'])) {

                if ($column['type'] == 'numeric') {
                    $column['value'] = str_replace('.', '', $column['value']);
                    $column['value'] = (float) str_replace(',', '.', $column['value']);
                }
                //
                if ($column['type'] == 'date') {
                    $dateExplode = explode("/", $column['value']);
                    $column['value'] = "'" . implode("-", array_reverse($dateExplode)) . "'";
                } elseif ($column['type'] == 'timestamp without time zone' && strpos($column['value'], '/')) {
                    $column['value'] = addslashes($column['value']);
                    if (strpos($column['value'], ':')) {
                        $dateExplode = explode(" ", $column['value']);
                        $dateExplode[0] = explode("/", $dateExplode[0]);
                        $column['value'] = "'" . implode("-", array_reverse($dateExplode[0])) . ' ' . $dateExplode[1] . "'";
                    } else {
                        $dateExplode = explode("/", $column['value']);
                        $column['value'] = "'" . implode("-", array_reverse($dateExplode)) . "'";
                    }
                } else {
                    $column['value'] = addslashes($column['value']);
                    $column['value'] = "'{$column['value']}'";
                }
            }
        }
    }

    /**
     * Trata os valores da entidade, para o usuario
     *
     * @name treatEntityToUser
     * @access public
     * @return void
     *
     * @author Junio Santo <junio.santos@mec.com>
     * @since 11/07/2014
     */
    public function treatEntityToUser() {
        foreach ($this->entity as $columnName => &$column) {
            $valor = $column['value'];

            if (!empty($valor)) {
                if ($column['type'] == 'numeric') {
                    $column['value'] = number_format($valor, 2, ',', '.');
                }

                /** DATE AND DATETIME  */
                if ($column['type'] == 'date') {
                    $column['value'] = date('d/m/Y', strtotime($valor));
                } elseif ($column['type'] == 'timestamp without time zone' && strpos($valor, '/')) {
                    $column['value'] = addslashes($valor);
                    if (strpos($column['value'], ':')) {
                        $column['value'] = date('d/m/Y H:m:s', strtotime($valor));
                    } else {
                        $column['value'] = date('d/m/Y', strtotime($valor));
                    }
                }
            }
        }
    }

    /**
     * Salva a entidade, mas antes de salvar realiza a validacao para manter a integridade dos dados com a tabela do banco de dados.
     *
     * @name isValid
     * @access public
     * @return boolean - true / false
     *
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.com>
     * @since 22/08/2013
     */
    public function save($validar = true) {
        // Se a entidade estiver valida e nao exisitr nenhuma mensagem de erro.
        if ($validar && !$this->isValid() && $this->error) {
            return false;
        }

        if ($this->checkPkValues()) {
            return $this->update(false);
        } else {
            return $this->insert(false);
        }
    }

    /**
     * Salva a entidade, mas antes de salvar realiza a validacao para manter a integridade dos dados com a tabela do banco de dados.
     *
     * @name insert
     * @param boolean $validar Se valida ou não
     * @access public
     * @return mixed - bool / pkValue
     *
     * @author Ruy Junior Ferreira Silva <ruyjfs@gmail.com>
     * @author Junio Santos <junio.santos@mec.com>
     * @since 24/07/2014
     */
    public function insert($validar = true, $inserirPks = false) {

        // Se a entidade estiver valida e nao exisitr nenhuma mensagem de erro.
        if ($validar && !$this->isValid() && $this->error) {
            return false;
        }

        $this->treatEntity();

        $entityValues = $this->getEntityValues($inserirPks);
        $sql = "INSERT INTO {$this->_names}";

        // Colocando nome das colunas da tabela.
        $sql .= "( " . implode(", ", array_keys($entityValues)) . " )";

        // Colocando valores referente às colunas da tabela.
        if($this->_decode){
            $sql .= " VALUES ( " . (implode(", ", $entityValues)) . " )";
        } else {
            $sql .= " VALUES ( " . implode(", ", $entityValues) . " )";
        }

        $sql .= $this->getSqlSelfId();

        $result = $this->_db->pegaUm($sql);

        if ($this->_commit) {
            $this->commit();
        }
        return $result;
    }

    /**
     * Salva a entidade, mas antes de salvar realiza a validacao para manter a integridade dos dados com a tabela do banco de dados.
     *
     * @name update
     * @param boolean $validar Se valida ou não
     * @access public
     * @return mixed - bool / pkValue
     *
     * @author Junio Santos <junio.santos@mec.com>
     * @author Ruy Junior Ferreira Silva <ruyjfs@gmail.com>
     * @since 24/07/2014
     */
    public function update($validar = true) {
        // Se a entidade estiver valida e nao exisitr nenhuma mensagem de erro.
        if ($validar && !$this->isValid() && $this->error) {
            return false;
        }

        $this->treatEntity();

        // Se tiver valor na PK significa que esta editando.
        if ($this->checkPkValues()) {
            $sql = "UPDATE {$this->_names} SET ";
            $sql .= $this->getSqlSetValues();
            $sql .= $this->getSqlWhere();
        }
        $sql .= $this->getSqlSelfId();

        $result = $this->_db->pegaUm($sql);
        if ($this->_commit) {
            $this->commit();
        }
        return $result;
    }

    private function getSqlSetValues() {
        $n = 0;
        $sql = '';
        foreach ($this->getEntityValues() as $key => $value) {
            if($this->_decode){
                $value = ($value);
            } else {
                $value = $value;
            }
            if ($n > 0) {
                $sql .= " , ";
            }
            $sql .= "{$key} = {$value}";
            $n++;
        }
        return $sql;
    }

    private function getSqlWhere() {
        $primary = $this->getPkValues();
        $sql = " WHERE ";
        $n = 0;
        foreach ($primary as $pk => $value) {
            if ($n > 0) {
                $sql .= ' AND ';
            }
            $sql .= " {$pk} = {$value}";
            $n++;
        }
        return $sql;
    }

    private function getSqlSelfId() {
        $primaryKeys = $this->getPkNames();
        return " RETURNING {$primaryKeys[0]}";
    }

    /**
     * Valida a entidade.
     * Pode ser chamada toda vez que necessitar saber se os dados populados na entidade estao corretos de acordo com a tabela do banco de dados.
     *
     * @name isValid
     * @access public
     * @return boolean - true / false
     *
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.com>
     * @since 22/08/2013
     */
    public function isValid() {

        foreach ($this->entity as $nameColumn => $column) {

            $validate = null;
            // Se for chave primaria e tiver valor, valida inteiro somente sem nenhuma outra validacao.
            if ($column['contraint'] == 'pk' && $column['value']) {
                $validate = new Zend_Validate_Digits();
            } else {

//______________________________________________________________________________ VALIDANDO POR TIPO DA COLUNA NO BANCO
                // Se tiver valor comeca validacoes de acordo com o tipo.
                // Se nao pergunta se é obrigatorio, se for obrigatorio valida.
                if ($column['value']) {
                    // Se for inteiro
                    if ($column['type'] == 'integer' || $column['type'] == 'smallint') {
                        $validate = new Zend_Validate_Digits();
                        // Se for data
                    } else if ($column['type'] == 'numeric') {
                        $column['value'] = str_replace('.', '', $column['value']);
                        $column['value'] = (float) str_replace(',', '.', $column['value']);
                        $validate = new Zend_Validate_Float();
                    } else if ($column['type'] == 'date') {

						$posBarra = strpos($column['value'], '/');
						$pos = strpos($column['value'], '-');
						if ($posBarra !== false) {
							$date = explode('/', $column['value']);
							if (!checkdate($date[1], $date[0], $date[2])) {
								$this->error[] = array("name" => $nameColumn, "msg" => $column['value'] . self::MSG_DATA_INVALIDA);
							}
						}
						if ($pos !== false) {
							$date = explode('-', $column['value']);
							if (!checkdate($date[1], $date[2], $date[0])) {
								$this->error[] = array("name" => $nameColumn, "msg" => $column['value'] . self::MSG_DATA_INVALIDA);
							}
						}

                        // Se for texto
                    } else if ($column['type'] == 'character' || $column['type'] == 'character varying') {

                        // Se for Bollean
                    } else if ($column['type'] == 'boolean') {

                        // Se for texto
                    } else if ($column['type'] == 'text') {

                    } else if ($column['type'] == 'timestamp without time zone') {
                        if (strpos($column['value'], '/')) {

                            if (strpos($column['value'], ' ')) {
                                $arrDate = explode(' ', $column['value']);

                                // Validando a data
                                $date = explode('/', $arrDate[0]);
                                if (!checkdate($date[1], $arrDate[0], $date[2])) {
                                    $this->error[] = array("name" => $nameColumn, "msg" => $arrDate[0] . self::MSG_DATA_INVALIDA);
                                }

                                // Validando a hora
                                $validateTime = new Zend_Validate_Date('hh:mm');
                                $validateTime->isValid($arrDate[1]);
                                if ($validateTime->getErrors()) {
                                    $this->error[] = array("name" => $nameColumn, "msg" => $arrDate[1] . self::MSG_DATA_HORA_INVALIDA);
                                }
                            } else {
                                // Validando a data
                                $date = explode('/', trim($column['value']));
                                if (!checkdate($date[1], $date[0], $date[2])) {
                                    $this->error[] = array("name" => $nameColumn, "msg" => $column['value'] . self::MSG_DATA_INVALIDA);
                                }
                            }
                        }
                    }

//______________________________________________________________________________ VALIDANDO POR LIMITE DA COLUNA NO BANCO
                    // Validando quantidade de caracteres.
                    if ($column['maximum']) {
                        $validateMax = new Zend_Validate_StringLength(array('max' => $column['maximum']));
                        $validateMax->isValid($column['value']);
                        if ($validateMax->getErrors()) {
                            $this->error[] = array("name" => $nameColumn, "msg" => reset($validateMax->getMessages()));
                        }
                        $validateMax = null;
                    }
                } else {
//______________________________________________________________________________ VALIDANDO POR NAO VAZIO DA COLUNA NO BANCO
                    // Validando se nao tiver valor e ele for obrigatorio.
                    if ($column['is_null'] == 'NO' && $column['contraint'] != 'pk') {
                        $validate = new Zend_Validate_NotEmpty();
                    }
                }

//______________________________________________________________________________ VALIDANDO A CLOUNA E COLOCANDO A MENSAGEM
                if ($validate) {
                    $validate->isValid($column['value']);
                    if ($validate->getErrors()) {
						$msg = $validate->getMessages();
                        $this->error[] = array("name" => $nameColumn, "msg" =>  reset($msg) );
                    }

                    $validate = null;
                }
            }
        }

        if ($this->error)
            return false;
        else
            return true;
    }

    public function getByValues(array $data) {
        $sql = "SELECT * FROM {$this->_names} WHERE ";

        $n = 0;
        foreach ($data as $key => $value) {
            if ($n > 0)
                $sql .= " AND ";
            $sql .= "{$key} = '{$value}'";
            $n++;
        }

        $result = $this->_db->pegaLinha($sql);
        return $result;
    }

    public function getAllByValues(array $data, array $ordersBy = null) {
        $sql = "SELECT * FROM {$this->_names} WHERE ";

        $n = 0;
        foreach ($data as $key => $value) {

            if(is_array($value) || $value != ''){
                $op = '=';
                $vlr = "'".$value."'";

                if(is_array($value)){
                    $op = 'IN';
                    $vlr = "(".implode(', ', $value).")";
                }

                if ($n > 0)
                    $sql .= " AND ";

                $sql .= "$key $op $vlr";
            }

            $n++;
        }

        if ($ordersBy) {
            $sql .=' ORDER BY ';

            $n = 0;
            foreach ($ordersBy as $orderBy) {
                if ($n > 0)
                    $sql .= " , ";
                $sql .= $orderBy;
            }
        }

        $result = $this->_db->carregar($sql);
        return ($result) ? $result : array();
    }

    public function getAll($limit = NULL, $ordersBy = NULL) {
        $sql = "SELECT * FROM {$this->_names} ";
        if ($limit) {
            $sql .= " LIMIT " . $limit;
        }

        if ($ordersBy) {
            $sql .=' ORDER BY ';

            $n = 0;
            foreach ($ordersBy as $orderBy) {
                if ($n > 0)
                    $sql .= " , ";
                $sql .= $orderBy;
            }
        }

        $result = $this->_db->carregar($sql);
        return $result;
    }

    public function fetchAll($where = array(), $order = null, $limit = NULL) {
        $sql = "select * from {$this->_names} ";
        $sql .= $where ? ' where ' . implode(' AND ', (array) $where) : '';
        $sql .= $order ? " order by  $order" : '';
        $sql .= $limit ? " limit  $limit" : '';

        return $this->_db->carregar($sql);
    }

    public function delete() {
        $primary = $this->getPkValues();
        $sql = "DELETE FROM {$this->_names} WHERE " . implode(',', array_keys($primary)) . " = " . implode(',', array_values($primary)) . "";
        $result = $this->_db->executar($sql);
        $this->_db->commit();

        return $result;
    }

    public function deleteAllByValues(array $values) {
        $conds = array();
        foreach($values as $key=>$valor){
            $conds[] = "{$key} = '{$valor}'";
        }
        $sql = "DELETE FROM {$this->_names} WHERE " . implode(' AND ', $conds) . "";
        $result = $this->_db->executar($sql);
        $this->_db->commit();

        return $result;
    }

    public function getAttributeValue($id) {
        return $this->entity[$id]['value'];
    }

    public function getAttributeLabel($id) {
        return $this->entity[$id]['label'];
    }

    public function setAttributeValue($id, $value) {
        $this->entity[$id]['value'] = $value;
    }

    function mask($val, $mask) {
        $maskared = '';
        $k = 0;
        for ($i = 0; $i <= strlen($mask) - 1; $i++) {
            if ($mask[$i] == '#') {
                if (isset($val[$k]))
                    $maskared .= $val[$k++];
            } else {
                if (isset($mask[$i]))
                    $maskared .= $mask[$i];
            }
        }
        return $maskared;
    }

    public function validarEmail($email) {
		if (!preg_match("/^[^@]{1,64}@[^@]{1,255}$/", $email)) {
			// Email invalid because wrong number of characters in one section, or wrong number of @ symbols.
			return false;
		}
		// Split it into sections to make life easier
		$email_array = explode("@", $email);
		$local_array = explode(".", $email_array[0]);
		for ($i = 0; $i < sizeof($local_array); $i++) {
			if (!preg_match("/^(([A-Za-z0-9!#$%&'*+\/=?^_`{|}~-][A-Za-z0-9!#$%&'*+\/=?^_`{|}~\.-]{0,63})|(\"[^(\\|\")]{0,62}\"))$/", $local_array[$i])) {
				return false;
			}
		}
		if (!preg_match("/^\[?[0-9\.]+\]?$/", $email_array[1])) { // Check if domain is IP. If not, it should be valid domain name
			$domain_array = explode(".", $email_array[1]);
			if (sizeof($domain_array) < 2) {
				return false; // Not enough parts to domain
			}
			for ($i = 0; $i < sizeof($domain_array); $i++) {
				if (!preg_match("/^(([A-Za-z0-9][A-Za-z0-9-]{0,61}[A-Za-z0-9])|([A-Za-z0-9]+))$/", $domain_array[$i])) {
					return false;
				}
			}
		}

		return true;
    }

    public function getOptions(array $dados, array $htmlOptions = array(), $idCampo = null, $descricaoCampo = null) {
        $html = '';
        $selected = '';

        if (isset($htmlOptions['prompt'])) {
            $value = ( $htmlOptions['prompt_value'] ? $htmlOptions['prompt_value'] : '' );
            $html.="<option value='{$value}'>" . strtr($htmlOptions['prompt'], array('<' => '&lt;', '>' => '&gt;')) . "</option>\n";
        }

        if ($dados) {
            foreach ($dados as $data) {
                $descricao = $data['descricao'];
                $codigo = $data['codigo'];
                if(empty($descricao)){
                    $descricao = $data[$descricaoCampo];
                }
                if(empty($codigo)){
                    $codigo = $data[$idCampo];
                }
                if ($idCampo) {
                    $selectedText = "selected='true' ";
					$selectedTextNew = '';

                    if( is_array( $this->getAttributeValue($idCampo) )){
                        $ids = $this->getAttributeValue($idCampo);
                        foreach( $ids as $id ){
                            if( $codigo == $id  ){
								$selectedTextNew = $selectedText;
                            }
                        }
                    }else{
                        if( $codigo == $this->getAttributeValue($idCampo)  ){
							$selectedTextNew = $selectedText;
                        }
                    }
                }
                $html .= "<option " . $selectedTextNew ." title='{$descricao}' value='{$codigo}'>{$descricao}</option>";
            }
        }
        return $html;
    }

    /**
     * Procura de acordo com a entity, onde numericos ficam com = e strings com ILIKE.
     *
     * @name search
     * @access public
     * @param array $data - Array onde as chaves tem que ser iguais a da entity.
     * @return boolean - true / false
     *
     * @author Ruy Junior Ferreira Silva <ruyjfs@gmail.com>
     * @since  21/10/2014
     */
    public function search(array $data)
    {
//        $this->searchWhere($data);
//        ver($this->entity, d);
    }

    /**
     * Procura de acordo com a entity, onde numericos ficam com = e strings com ILIKE.
     *
     * @name search
     * @access public
     * @param array $data - Array onde as chaves tem que ser iguais a da entity.
     * @return string - $where
     *
     * @author Ruy Junior Ferreira Silva <ruyjfs@gmail.com>
     * @since  21/10/2014
     */
    public function searchWhere(array $data)
    {
        $this->populateEntity($data);

        $where = array();

        foreach($data as $key => $value){
            if(strripos($key , '_begin')){
                $dateColumn = str_replace('_begin' , '', $key);
                $dateBegin = $key;
            } else if(strripos($key , '_end')) {
                $dateEnd = $key;
            }
        }

        if($dateBegin && $dateEnd && !empty($data[$dateBegin]) && !empty($data[$dateEnd])){
            $dataWhere[] = " ({$dateColumn} BETWEEN  TO_DATE('{$data[$dateBegin]}' , 'DD/MM/YYYY') AND TO_DATE('{$data[$dateEnd]}' , 'DD/MM/YYYY'))";
        }

        foreach($this->entity as $nameColumn => $entity){
            if(!empty($entity['value']) ){
                if($entity['type'] == 'integer' || $entity['type'] == 'numeric'){
                    $dataWhere[] = "{$nameColumn} = '{$entity['value']}'";
                } else if ($entity['type'] == 'date' || $entity['type'] == 'timestamp with time zone'){
                    $dataWhere[] = "TO_CHAR({$nameColumn} , 'DD/MM/YYYY') = '{$entity['value']}'";
                } else {
                    $dataWhere[] = "public.removeacento({$nameColumn}) ILIKE public.removeacento('%{$entity['value']}%')";
                }
            }
        }

        if( $dataWhere && count($dataWhere) > 0 ) {
            $where = implode(' AND ' , $dataWhere );
        } else {
            $where = '';
        }

        $this->clearEntity();

        return $where;
    }


}
