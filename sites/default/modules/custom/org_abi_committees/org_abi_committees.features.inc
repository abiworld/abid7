<?php
/**
 * @file
 * 
 */

/**
 * Implements hook_ctools_plugin_api().
 * 
 * @see hook_ctools_plugin_api()
 */
function org_abi_committees_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == 'relation' && $api == 'relation_type_default') {
    return array(
      'version' => '1',
    );
  }
}

/**
 * Implements hook_taxonomy_default_vocabularies().
 *
 * @see hook_taxonomy_default_vocabularies()
 */
function org_abi_committees_taxonomy_default_vocabularies() {
  return array(
    'committees' => array(
      'name' => 'Committees',
      'machine_name' => 'committees',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'topics' => array(
      'name' => 'Topics',
      'machine_name' => 'topics',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_uuid_features_default_terms().
 *
 * @see hook_uuid_features_default_terms()
 */
function org_abi_committees_uuid_features_default_terms() {
  $terms = array();

  // Committees.
  $terms[] = array(
    'name' => 'Technology and Intellectual Property',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '01d54d1e-7c1c-4393-9caf-34ce3fa209c6',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Young and New Members',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '24c0ed6f-9e5c-4fc6-aecc-203506fa29d2',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Unsecured Trade Creditors',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '36190f71-f15a-4aa4-aba5-5cf672349a7a',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Real Estate',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '5692f653-b84f-4d67-9598-c3592a9d7117',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Consumer Bankruptcy',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '7b446015-8409-4058-b65c-e7839bb274cb',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Business Reorganization',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '8191983f-f5f2-4f04-9eeb-311f99c72cfd',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Bankruptcy Taxation',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '918f1139-ff65-4801-9637-7ad931cf17cf',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Asset Sales',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '93f67c56-d2fb-48dd-bc38-10619a733253',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Ethics and Professional Compensation',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '969fdf6f-28b8-43f3-a7a5-cea6b27ed052',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Legislation',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'abcd72e6-4e4c-457c-813e-bb022f86f8ba',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Labor and Employment',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'b1a49573-09fa-4f51-9ac4-e9ce99459b00',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Secured Credit',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'b7cf0eda-a6b6-4891-92b8-9f5f5930d3a6',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Financial Advisors and Investment Banking',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'c54c0a55-1255-4c55-8dba-1faaaad9c2ef',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Commercial Fraud',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'c63886d7-15be-46f9-802a-a0abb71d2bbc',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Mediation',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'e828773c-ce92-486d-a2b5-cde56f569999',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Bankruptcy Litigation',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'f0dc3825-1d0b-4945-8fc5-8c3ca0807538',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'International',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'fba432ea-855c-4427-b4c6-80f66748dac4',
    'vocabulary_machine_name' => 'committees',
  );
  $terms[] = array(
    'name' => 'Health Care',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'fdd5a50b-22e3-4f49-9bf2-68dd3ed30b88',
    'vocabulary_machine_name' => 'committees',
  );

  // Topics.
  $terms[] = array(
    'name' => 'Executory Contracts/Leases',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '0b7c3d7f-c60f-400d-953c-b801ceac2c20',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Uniform Commercial Code/Article 9',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '0d58872c-c131-4f81-8fcf-bb6088c7c861',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Investment Banking',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '0db18a2a-dbf0-4fa0-bc3e-773b90a628b9',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Financial Advisors',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '0e96de28-b173-4018-a015-f860e6ccb3c6',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Plan Confirmation',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '0ffacf27-2950-4e82-bb8b-eea692de1a8a',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'International Insolvency',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '11ff31e4-5547-4bbb-a059-76bc28377fb0',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Bankruptcy Taxation',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '134399e8-ab5a-4b21-8cd0-eaed67c8be8a',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Administrative Expense Claims',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '18f8cd28-dffc-43f5-a0d1-74bbe2f5d68a',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Preferences',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '191b2ff3-b38e-4c05-a250-cf3ca72619d5',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Fraud',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '19b083a6-1530-41fe-a036-bd60ead67d26',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Creditors\' Committee',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '1e2d0bc4-b706-4535-bd65-b7f914fada95',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Legislation',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '217421c2-2a6b-4469-904a-dcf2b67e90b3',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'First-day Motions',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '22a9b688-b0cc-49b9-b5cb-212bd18b3783',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Insurance',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '2337cfab-3c38-4fa8-bd15-cbaa8828a5b1',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Automatic Stay',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '2b433ddb-23c7-4f51-a72e-57d706a93f0b',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Discharge/Dischargeability',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '2db812c8-7cb7-42cf-ad56-8dc8746e878d',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bf97630d-70d6-4740-8a20-b1c261cb4494',
    ),
  );
  $terms[] = array(
    'name' => 'Finance and Banking',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '32d59855-379b-459d-818e-3e73a9842fe1',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Lender Liability',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '3c301d30-2d61-4583-a968-198bc052b796',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Professional Practice Areas',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    'vocabulary_machine_name' => 'topics',
  );
  $terms[] = array(
    'name' => 'Consumer Debt',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '3ee07ddb-504a-4937-be32-7fa796624add',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bf97630d-70d6-4740-8a20-b1c261cb4494',
    ),
  );
  $terms[] = array(
    'name' => 'Venue/Jurisdiction',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '432d2184-8519-432d-8fec-7e4699e7bdf5',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Alternative Dispute Resolution',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '4772e3bd-bec5-4ee5-946e-be808cfe86e5',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Fraudulent Transfers',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '4bcae8d5-d1b7-4844-96ed-4fce983f2165',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Govt. Claims/Sovereign Immunity',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '4eb0ca76-b74b-48e7-bc6d-e382947d54bf',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Hospitality, Entertainment Venues and Gaming',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '52b778ba-2be9-44f9-8020-99fa8be85281',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Consumer Bankruptcy',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '5a146fb2-584f-4ce8-8d52-8c466604171b',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bf97630d-70d6-4740-8a20-b1c261cb4494',
    ),
  );
  $terms[] = array(
    'name' => 'Public Companies and Claims Trading',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '5c60df6f-b643-4c16-bc46-35f6a041242b',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Professional Compensation/Fees',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '5fc4e3fd-4f73-4fbd-af64-0e608f890b19',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Retail',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '609def6a-72e8-4cf4-8c25-1d9bf329c4d4',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'SEC Issues',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '623abad9-4e12-4268-9580-6a232e061a89',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Small Business',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '63e1b527-ed2c-4ad5-a907-96cb6093b997',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Business Reorganization',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '655c8863-dab2-4fd1-ba62-7baaa4e299a8',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Unsecured Trade Creditors',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '78497a97-a829-4aaf-8af9-b217f3b42b4c',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Agriculture/PACA',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '78dc6274-4c0d-4356-b781-67efafe65497',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Family/Divorce Issues',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '7b7c0b03-ece2-4458-b50e-d5145c7d98da',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bf97630d-70d6-4740-8a20-b1c261cb4494',
    ),
  );
  $terms[] = array(
    'name' => 'Airlines',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '81523894-2e03-4346-a11b-11cdf9513023',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Asset Sales',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '915f1dc2-a176-43e2-afa2-435c5159bd78',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Municipal Bankruptcy',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => '925b4a5b-b856-417e-ac73-da9c33da3df0',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Turnaround Issues',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'a0566d9c-6754-40c3-93fe-dc07384ff78b',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Real Estate',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'a2542a8a-b445-4e7f-bef6-3f353a3b7385',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Claims',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'a7684043-cfb5-47f7-aaab-71361c7cc534',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Chapter 12/Farm Cases',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'a7ba556a-5fa1-40ec-af0d-9f08fd61f5e8',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Telecom/Tech',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'be7c239b-16c9-49fe-9648-d8fbe3fcca80',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Practice and Procedure',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'bf1d8546-8140-4e03-a64e-ed534e6bfda0',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Personal Bankruptcy Topics',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'bf97630d-70d6-4740-8a20-b1c261cb4494',
    'vocabulary_machine_name' => 'topics',
  );
  $terms[] = array(
    'name' => 'Bankruptcy Process and Procedure',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'bfed745e-477e-49af-86c8-e1efe3274898',
    'vocabulary_machine_name' => 'topics',
  );
  $terms[] = array(
    'name' => 'Ethics',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'c0c6de39-f2b0-4864-b239-642e6dbf8496',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Mass Torts',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'c52bce72-b636-4d28-8793-46e2b9283d79',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Corporate Governance',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'ca2e4e68-8cdc-4870-8c31-c338257310f3',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Valuation',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'cf3edfe4-69b0-436f-9a8e-d6369ee78b0d',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Certification',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'd8c6cc60-12fc-44fb-a6a7-0173c001305f',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Labor/Employment',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'da014397-43ce-45a6-86f0-bb71d8755671',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Bankruptcy Litigation',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'df907668-4c70-4c96-b706-1f1a294c23de',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );
  $terms[] = array(
    'name' => 'Mortgage',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'e3e6b665-7726-42a2-a1df-b0a801db5965',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bf97630d-70d6-4740-8a20-b1c261cb4494',
    ),
  );
  $terms[] = array(
    'name' => 'Health Care',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'e557e9fd-417d-4576-b2c3-25bb1ce05023',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Commercial Fraud',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'e71e905d-f6aa-48bd-8e50-f79adbdf046c',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => '3d79106f-1840-417d-ba01-c4e8172b8c14',
    ),
  );
  $terms[] = array(
    'name' => 'Predatory Lending',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'f3865bec-78f0-412b-8b93-7e06d05c84a7',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bf97630d-70d6-4740-8a20-b1c261cb4494',
    ),
  );
  $terms[] = array(
    'name' => 'Court Administration',
    'description' => NULL,
    'format' => NULL,
    'weight' => 0,
    'uuid' => 'f4ad0b04-f08f-4c09-b103-514fd565d6d7',
    'vocabulary_machine_name' => 'topics',
    'parent' => array(
      0 => 'bfed745e-477e-49af-86c8-e1efe3274898',
    ),
  );

  return $terms;
}

/**
 * Implements hook_relation_default_relation_types().
 *
 * @see hook_relation_default_relation_types()
 */
function org_abi_committees_relation_default_relation_types() {
  $export = array();

  $relation_type = new stdClass();
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'committee_tagged_with';
  $relation_type->label = 'committee tagged with';
  $relation_type->reverse_label = 'tag(s) pertain(s) to committee';
  $relation_type->directional = 1;
  $relation_type->transitive = 1;
  $relation_type->r_unique = 0;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 0;
  $relation_type->source_bundles = array(
    0 => 'taxonomy_term:*',
  );
  $relation_type->target_bundles = array(
    0 => 'taxonomy_term:*',
  );
  $export['committee_tagged_with'] = $relation_type;

  return $export;
}
