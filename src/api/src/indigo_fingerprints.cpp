/****************************************************************************
 * Copyright (C) 2010-2011 GGA Software Services LLC
 *
 * This file is part of Indigo toolkit.
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 3 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ***************************************************************************/

#include "indigo_fingerprints.h"
#include "molecule/molecule_fingerprint.h"
#include "base_cpp/output.h"
#include "base_c/bitarray.h"
#include "reaction/reaction_fingerprint.h"
#include "base_cpp/auto_ptr.h"
#include "reaction/reaction.h"
#include "indigo_molecule.h"
#include "indigo_reaction.h"
#include "base_cpp/scanner.h"

IndigoFingerprint::IndigoFingerprint () : IndigoObject(FINGERPRINT)
{
}

IndigoFingerprint::~IndigoFingerprint ()
{
}

IndigoFingerprint & IndigoFingerprint::cast (IndigoObject &obj)
{
   if (obj.type == IndigoObject::FINGERPRINT)
      return (IndigoFingerprint &)obj;
   throw IndigoError("%s is not a fingerprint", obj.debugInfo());
}

void _indigoParseMoleculeFingerprintType (MoleculeFingerprintBuilder &builder, const char *type,
                                          bool query)
{
   builder.query = query;
   
   if (type == 0 || *type == 0 || strcasecmp(type, "sim") == 0)
   {
      // similarity
      builder.skip_tau = true;
      builder.skip_ext = true;
      builder.skip_ord = true;
      builder.skip_any_atoms = true;
      builder.skip_any_bonds = true;
      builder.skip_any_atoms_bonds = true;
   }
   else if (strcasecmp(type, "sub") == 0)
   {
      // substructure
      builder.skip_sim = true;
      builder.skip_tau = true;
   }
   else if (strcasecmp(type, "sub-res") == 0)
   {
      // resonance substructure
      builder.skip_sim = true;
      builder.skip_tau = true;
      builder.skip_ord = true;
      builder.skip_any_atoms = true;
      builder.skip_ext_charge = true;
   }
   else if (strcasecmp(type, "sub-tau") == 0)
   {
      // tautomer
      builder.skip_ord = true;
      builder.skip_sim = true;

      // tautomer fingerprint part does already contain all necessary any-bits
      builder.skip_any_atoms = true;
      builder.skip_any_bonds = true;
      builder.skip_any_atoms_bonds = true;
   }
   else if (strcasecmp(type, "full") == 0)
   {
      if (query)
         throw IndigoError("there can not be 'full' fingerprint of a query molecule");
      // full (non-query) fingerprint, do not skip anything
   }
   else
      throw IndigoError("unknown molecule fingerprint type: %s", type);
}

void _indigoParseReactionFingerprintType (ReactionFingerprintBuilder &builder, const char *type,
                                          bool query)
{
   builder.query = query;

   if (type == 0 || *type == 0 || strcasecmp(type, "sim") == 0)
   {
      // similarity
      builder.skip_ext = true;
      builder.skip_ord = true;
   }
   else if (strcasecmp(type, "sub") == 0)
      // substructure
      builder.skip_sim = true;
   else if (strcasecmp(type, "full") == 0)
   {
      if (query)
         throw IndigoError("there can not be 'full' fingerprint of a query reaction");
      // full (non-query) fingerprint, do not skip anything
   }
   else
      throw IndigoError("unknown molecule fingerprint type: %s", type);
}

CEXPORT int indigoFingerprint (int item, const char *type)
{
   INDIGO_BEGIN
   {
      IndigoObject &obj = self.getObject(item);

      if (IndigoBaseMolecule::is(obj))
      {
         BaseMolecule &mol = obj.getBaseMolecule();
         MoleculeFingerprintBuilder builder(mol, self.fp_params);

         _indigoParseMoleculeFingerprintType(builder, type, mol.isQueryMolecule());
         builder.process();
         AutoPtr<IndigoFingerprint> fp(new IndigoFingerprint());
         fp->bytes.copy(builder.get(), self.fp_params.fingerprintSize());
         return self.addObject(fp.release());
      }
      else if (IndigoBaseReaction::is(obj))
      {
         BaseReaction &rxn = obj.getBaseReaction();
         ReactionFingerprintBuilder builder(rxn, self.fp_params);

         _indigoParseReactionFingerprintType(builder, type, rxn.isQueryReaction());
         builder.process();
         AutoPtr<IndigoFingerprint> fp(new IndigoFingerprint());
         fp->bytes.copy(builder.get(), self.fp_params.fingerprintSizeExtOrdSim() * 2);
         return self.addObject(fp.release());
      }
      else
         throw IndigoError("indigoFingerprint(): accepting only molecules and reactions, got %s", obj.debugInfo());
      return 1;
   }
   INDIGO_END(-1);
}

void IndigoFingerprint::toString (Array<char> &str)
{
   ArrayOutput output(str);
   int i;

   for (i = 0; i < bytes.size(); i++)
      output.printf("%02x", bytes[i]);
}

void IndigoFingerprint::toBuffer (Array<char> &buf)
{
   buf.copy((char *)bytes.ptr(), bytes.size());

}

float _indigoSimilarity2 (const byte *arr1, const byte *arr2, int size, const char *metrics)
{
   int ones1 = bitGetOnesCount(arr1, size);
   int ones2 = bitGetOnesCount(arr2, size);
   int common_ones = bitCommonOnes(arr1, arr2, size);

   if (metrics == 0 || metrics[0] == 0 || strcasecmp(metrics, "tanimoto") == 0)
   {
      if (common_ones == 0)
         return 0;

      return (float)common_ones / (ones1 + ones2 - common_ones);
   }
   else if (strlen(metrics) >= 7 && strncasecmp(metrics, "tversky", 7) == 0)
   {
      float alpha = 0.5f, beta = 0.5f;

      const char *params = metrics + 7;

      if (*params != 0)
      {
         BufferScanner scanner(params);
         if (!scanner.tryReadFloat(alpha))
            throw IndigoError("unknown metrics: %s", metrics);
         scanner.skipSpace();
         if (!scanner.tryReadFloat(beta))
            throw IndigoError("unknown metrics: %s", metrics);
      }
      if (common_ones == 0)
         return 0;

      float denom = (ones1 - common_ones) * alpha + (ones2 - common_ones) * beta + common_ones;

      if (denom < 1e-6f)
         throw IndigoError("bad denominator");

      return common_ones / denom;
   }
   else if (strcasecmp(metrics, "euclid-sub") == 0)
   {
      if (common_ones == 0)
         return 0;

      return (float)common_ones / ones1;
   }
   else
      throw IndigoError("unknown metrics: %s", metrics);
}

float _indigoSimilarity (Array<byte> &arr1, Array<byte> &arr2, const char *metrics)
{
   int size = arr1.size();

   if (size != arr2.size())
      throw IndigoError("fingerprint sizes do not match (%d and %d)", arr1.size(), arr2.size());

   return _indigoSimilarity2(arr1.ptr(), arr2.ptr(), size, metrics);
}


CEXPORT float indigoSimilarity (int item1, int item2, const char *metrics)
{
   INDIGO_BEGIN
   {
      IndigoObject &obj1 = self.getObject(item1);
      IndigoObject &obj2 = self.getObject(item2);

      if (IndigoBaseMolecule::is(obj1))
      {
         Molecule &mol1 = obj1.getMolecule();
         Molecule &mol2 = obj2.getMolecule();

         MoleculeFingerprintBuilder builder1(mol1, self.fp_params);
         MoleculeFingerprintBuilder builder2(mol2, self.fp_params);

         _indigoParseMoleculeFingerprintType(builder1, "sim", false);
         _indigoParseMoleculeFingerprintType(builder2, "sim", false);

         builder1.process();
         builder2.process();
         
         return _indigoSimilarity2(builder1.getSim(), builder2.getSim(),
                                   self.fp_params.fingerprintSizeSim(), metrics);
      }
      else if (IndigoBaseReaction::is(obj1))
      {
         Reaction &rxn1 = obj1.getReaction();
         Reaction &rxn2 = obj2.getReaction();

         ReactionFingerprintBuilder builder1(rxn1, self.fp_params);
         ReactionFingerprintBuilder builder2(rxn2, self.fp_params);

         _indigoParseReactionFingerprintType(builder1, "sim", false);
         _indigoParseReactionFingerprintType(builder2, "sim", false);

         builder1.process();
         builder2.process();

         return _indigoSimilarity2(builder1.getSim(), builder2.getSim(),
                                   self.fp_params.fingerprintSizeSim() * 2, metrics);
      }
      else if (self.getObject(item1).type == IndigoObject::FINGERPRINT)
      {
         IndigoFingerprint &fp1 = IndigoFingerprint::cast(obj1);
         IndigoFingerprint &fp2 = IndigoFingerprint::cast(obj2);

         return _indigoSimilarity(fp1.bytes, fp2.bytes, metrics);
      }
      else
         throw IndigoError("indigoSimilarity(): can not accept %s", obj1.debugInfo());
   }
   INDIGO_END(-1);
}

CEXPORT int indigoCountBits (int fingerprint)
{
   INDIGO_BEGIN
   {
      IndigoFingerprint &fp = IndigoFingerprint::cast(self.getObject(fingerprint));
      return bitGetOnesCount(fp.bytes.ptr(), fp.bytes.size());
   }
   INDIGO_END(-1);
}

CEXPORT int indigoCommonBits (int fingerprint1, int fingerprint2)
{
   INDIGO_BEGIN
   {
      Array<byte> &fp1 = IndigoFingerprint::cast(self.getObject(fingerprint1)).bytes;
      Array<byte> &fp2 = IndigoFingerprint::cast(self.getObject(fingerprint2)).bytes;

      if (fp1.size() != fp2.size())
         throw IndigoError("fingerprint sizes do not match (%d and %d)", fp1.size(), fp2.size());

      return bitCommonOnes(fp1.ptr(), fp2.ptr(), fp1.size());
   }
   INDIGO_END(-1);
}
